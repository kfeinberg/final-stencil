#include "Scene.h"

using namespace CS123::GL;

Scene::Scene()
{
    // creating primitives
    m_ground = std::make_unique<Plane>();
    m_grass = std::make_unique<Grass>();

    // loading and compiling phong shader
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/shader.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/shader.frag");
    m_shader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    // loading and compiling crepscular ray shader
    vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/volumetric.vert");
    fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/volumetric.frag");
    m_crepscularRayShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    // loading grass texture
    QImage grassImage(":/images/grass.png");
    m_grassTexture = std::make_unique<Texture2D>(grassImage.bits(), grassImage.width(), grassImage.height());
    TextureParametersBuilder builder;
    builder.setFilter(TextureParameters::FILTER_METHOD::NEAREST);
    builder.setWrap(TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE);
    TextureParameters params = builder.build();
    params.applyTo(*m_grassTexture.get());

    // black material for occluded scene
    m_occludedMaterial.clear();
    m_occludedMaterial.cAmbient.r = 0.f;
    m_occludedMaterial.cAmbient.g = 0.f;
    m_occludedMaterial.cAmbient.b = 0.f;

    // set wood material
    m_woodMaterial.clear();
    m_woodMaterial.cAmbient.r = 133.f / 255.f;
    m_woodMaterial.cAmbient.g = 94.f / 255.f;
    m_woodMaterial.cAmbient.b = 66.f / 255.f;

    // set leaf material
    m_leafMaterial.clear();
    m_leafMaterial.cAmbient.r = 14.f / 255.f;
    m_leafMaterial.cAmbient.g = 107.f / 255.f;
    m_leafMaterial.cAmbient.b = 14.f / 255.f;

    // initialize quad used for crepscular rays
    std::vector<GLfloat> quadData;
    quadData = {-1, 1, 0, 0, 1, -1, -1, 0, 0, 0, 1, 1, 0, 1, 1, 1, -1, 0, 1, 0};
    m_quad = std::make_unique<OpenGLShape>();
    m_quad->setVertexData(&quadData[0], quadData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_quad->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->setAttribute(ShaderAttrib::TEXCOORD0, 2, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->buildVAO();

    // initialize FBO to hold passes used for crepscular rays
    // TODO: figure out width and height
//    m_occludedPass = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY,
//                                        w, h, TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE);
//    m_regularPass = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY,
//                                        w, h, TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE);
}

Scene::~Scene()
{
}

Camera *Scene::getCamera() {
    return &m_camera;
}

void Scene::drawTree() {
    // source: https://github.com/abiusx/L3D/blob/master/L%2B%2B/tree.l%2B%2B
    std::map<char, std::string> rules;

    rules['A']="^FB//B*/////B";
    rules['B']="[^^F//////A]";

    LSystem l = LSystem(rules, "FA");

    std::string res = l.applyRules(9);

    Turtle t = Turtle();
    t.parseInput(res);

    Cylinder branch(8, 8);
    Leaf leaf;

    for (size_t i = 0; i < t.m_cylinderTransformations.size(); i++) {
        m_shader->setUniform("m", t.m_cylinderTransformations[i]);
        if (t.m_treeComponents[i] == TreeComponents::BRANCH) {
            m_shader->applyMaterial(m_woodMaterial);
            branch.draw();
        }
        else {
            m_shader->applyMaterial(m_leafMaterial);
            leaf.draw();
        }
    }
}

void Scene::crepscularRayPass() {

    // part 1: render scene normally
    m_regularPass->bind();
    // draw all shapes normally
    m_regularPass->unbind(); // unbind regular scene FBO

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear in between passes
    m_regularPass->getColorAttachment(0).bind(); // TODO: what does this do?

    // part 2: render scene with occluded objects
    m_occludedPass->bind();
    m_shader->applyMaterial(m_occludedMaterial);
    // draw all shapes with occluded material color
    m_occludedPass->unbind(); // unbind occluded pass scene FBO

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear in between passes
    m_occludedPass->getColorAttachment(0).bind(); // TODO: what does this do?

    // part 3: render combined scenes with crepscular rays
    // shader for crepscular rays
    m_crepscularRayShader->bind();
    // sets texture for two previously rendered scenes
    m_crepscularRayShader->setTexture("regularScene", m_regularPass->getColorAttachment(0)); // sets regular scene
    m_crepscularRayShader->setTexture("occludedScene", m_occludedPass->getColorAttachment(0));
    // set sun position
    m_crepscularRayShader->setUniform("sunPos", glm::vec3(1, 1, 1)); // TODO: update with real sun pos
    //glViewport(0, 0, m_width, m_height); TODO: do I need this?
    m_quad->draw(); // renders combined image with crespcular rays
}


void Scene::groundPass() {
    m_shader->bind();
    m_shader->setUniform("useLighting", false);
    m_shader->setUniform("useTexture", false);
    m_shader->setUniform("p", m_camera.getProjectionMatrix());
    m_shader->setUniform("v", m_camera.getViewMatrix());

    glm::mat4 m;
    m = m * glm::rotate(glm::half_pi<float>(), glm::vec3(1.f, 0.f, 0.f));
    m = m * glm::scale(glm::vec3(5.f, 5.f, 1.f));

    m_shader->setUniform("m", m);
    m_shader->applyMaterial(m_leafMaterial);
    m_ground->draw();
    m_shader->unbind();
}

void Scene::grassPass() {
    m_shader->bind();
    m_grassTexture->bind();
    m_shader->setUniform("useLighting", false);
    m_shader->setUniform("useTexture", true);
    m_shader->setUniform("p", m_camera.getProjectionMatrix());
    m_shader->setUniform("v", m_camera.getViewMatrix());

    glm::mat4 m;
    m = m * glm::translate(glm::vec3(0.f, .6f, 0.f));
    m = m * glm::scale(glm::vec3(.5f, .5f, .5f));

    m_shader->setUniform("m", m);
    m_shader->applyMaterial(m_leafMaterial);
    m_grass->draw();
    m_grassTexture->unbind();
    m_shader->unbind();
}

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // setting camera orientation
    m_camera.orientLook(
                glm::vec4(0.f, 1.f, 2.f, 1.f), // eye position
                glm::vec4(0.f, 0.f, -1.f, 0.f), // look vector
                glm::vec4(0.f, 1.f, 0.f, 0.f)); // up vector

    groundPass();
    grassPass();
}
