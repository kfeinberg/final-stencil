#include "Scene.h"

using namespace CS123::GL;

Scene::Scene()
{
    // creating primitives
    m_ground = std::make_unique<Plane>();
    m_grass = std::make_unique<Grass>();
    m_branch = std::make_unique<Cylinder>(8, 8);
    m_leaf = std::make_unique<Leaf>();
    m_sun = std::make_unique<Sphere>(8, 8);

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

    // setting camera orientation
    m_camera.orientLook(
                glm::vec4(0.f, 0.f, 20.f, 1.f), // eye position
                glm::vec4(0.f, 0.f, -1.f, 0.f), // look vector
                glm::vec4(0.f, 1.f, 0.f, 0.f)); // up vector

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

    // set white material
    m_whiteMaterial.clear();
    m_whiteMaterial.cAmbient.r = 255.f;
    m_whiteMaterial.cAmbient.g = 255.f;
    m_whiteMaterial.cAmbient.b = 255.f;

    // initialize quad used for crepscular ray display
    std::vector<GLfloat> quadData;
    quadData = {-1, 1, 0, 0, 1, -1, -1, 0, 0, 0, 1, 1, 0, 1, 1, 1, -1, 0, 1, 0};
    m_quad = std::make_unique<OpenGLShape>();
    m_quad->setVertexData(&quadData[0], quadData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_quad->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->setAttribute(ShaderAttrib::TEXCOORD0, 2, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->buildVAO();

    initializeTrees();
}

Scene::~Scene()
{
}

/**
 * @brief Initialize trees in m_trees to use throughout scene.
 */
void Scene::initializeTrees() {
    // source: https://github.com/abiusx/L3D/blob/master/L%2B%2B/tree.l%2B%2B
    std::map<char, std::string> rules;

    rules['A']="^FB//B*/////B";
    rules['B']="[^^F//////A]";

    std::string axiom = "FA";

    Tree t = Tree(rules, axiom, 10);
    m_trees.push_back(t);

    axiom = "FFF>FFFA";
    //axiom = "F";
    rules.clear();
    rules['A']="F[>++Al][>--Al]///A";
    t = Tree(rules, axiom, 7);

    m_trees.push_back(t);
}

void Scene::updateDimensions(int width, int height) {
    m_width = width;
    m_height = height;

    // update FBOs that hold passes used for crepscular rays
    m_occludedPass = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY,
                                         m_width, m_height, TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE);
    m_regularPass = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY,
                                         m_width, m_height, TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE);
}

CamtransCamera *Scene::getCamera() {
    return &m_camera;
}

void Scene::crepscularRayPass() {

    // part 1: render scene normally
    m_regularPass->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear in between passes

    // part 1.1: draw all shapes normally with phong shader
    glViewport(0, 0, m_width, m_height);
    renderPrimitives(false); // renders normal primitives
    // end of part 1.1

    m_regularPass->unbind();
    // end of part 1

    // part 2: render scene with occluded objects
    m_occludedPass->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear in between passes

    // part 2.1: draw all shapes with occluded material color with phong shader
    glViewport(0, 0, m_width, m_height);
    renderPrimitives(true); // renders occluded primitives
    // end of part 2.1

    m_occludedPass->unbind(); // unbind occluded pass scene FBO
    // end of part 2


    // part 3: render combined scenes with crepscular rays
    m_crepscularRayShader->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear in between passes

    // sets texture for two previously rendered scenes
    m_crepscularRayShader->setTexture("regularScene", m_regularPass->getColorAttachment(0)); // sets regular scene
    m_crepscularRayShader->setTexture("occludedScene", m_occludedPass->getColorAttachment(0));
    // set sun position
    m_crepscularRayShader->setUniform("sunPos", glm::vec3(0, .8f, -1.f)); // TODO: update with real sun pos

    glViewport(0, 0, m_width, m_height);
    m_quad->draw(); // renders combined image with crespcular rays

    m_crepscularRayShader->unbind();
}

void Scene::renderPrimitives(bool occluded) {
    // TODO: move primitives around!
    grassPass(occluded);
    groundPass(occluded);
    treePass(occluded);
    sunPass();
}


void Scene::groundPass(bool occluded) {
    m_shader->bind();
    m_shader->setUniform("useLighting", false);
    m_shader->setUniform("useTexture", false);
    m_shader->setUniform("p", m_camera.getProjectionMatrix());
    m_shader->setUniform("v", m_camera.getViewMatrix());

    glm::mat4 m;
    m = m * glm::rotate(glm::half_pi<float>(), glm::vec3(1.f, 0.f, 0.f));
    m = m * glm::scale(glm::vec3(5.f, 5.f, 5.f));

    m_shader->setUniform("m", m);
    if (occluded) {
         m_shader->applyMaterial(m_occludedMaterial);
    }
    else {
        m_shader->applyMaterial(m_leafMaterial);
    }
    m_ground->draw();
    m_shader->unbind();
}

void Scene::grassPass(bool occluded) {
    m_shader->bind();
    m_grassTexture->bind();
    m_shader->setUniform("useLighting", false);
    m_shader->setUniform("p", m_camera.getProjectionMatrix());
    m_shader->setUniform("v", m_camera.getViewMatrix());

    glm::mat4 m;
    m = m * glm::translate(glm::vec3(0.f, 0.5f, 0.f));
    m = m * glm::scale(glm::vec3(1.f, 1.f, 1.f));
    m_shader->setUniform("m", m);
    m_shader->setUniform("useTexture", true);

    if (occluded) {
        m_shader->applyMaterial(m_occludedMaterial);
        m_shader->setUniform("isOccluded", true);
    }
    else {
        m_shader->applyMaterial(m_leafMaterial);
    }

    m_grass->draw();
    m_grassTexture->unbind();
    m_shader->unbind();
}

void Scene::treePass(bool occluded) {
    m_shader->bind();
    m_shader->setUniform("useLighting", false);
    m_shader->setUniform("useTexture", false);
    m_shader->setUniform("p", m_camera.getProjectionMatrix());
    m_shader->setUniform("v", m_camera.getViewMatrix());

    Tree curr = m_trees[0];
    std::vector<glm::mat4x4> trans = curr.getTranformations();
    std::vector<TreeComponents> comps = curr.getComponents();

    for (size_t i = 0; i < trans.size(); i++) {
        m_shader->setUniform("m", trans[i]);
        if (comps[i] == TreeComponents::BRANCH) {
            if (occluded) {
                 m_shader->applyMaterial(m_occludedMaterial);
            }
            else {
                m_shader->applyMaterial(m_woodMaterial);
            }
            m_branch->draw();
        }
        else {
            if (occluded) {
                 m_shader->applyMaterial(m_occludedMaterial);
            }
            else {
                m_shader->applyMaterial(m_leafMaterial);
            }
            m_leaf->draw();
        }
    }
    m_shader->unbind();
}

void Scene::sunPass() {
    m_shader->bind();

    m_shader->setUniform("p", m_camera.getProjectionMatrix());
    m_shader->setUniform("v", m_camera.getViewMatrix());
    m_shader->setUniform("useLighting", true);
    m_shader->setUniform("useTexture", false);

    glm::mat4 m;
    m = m * glm::translate(glm::vec3(1.f, 5.3f, -4.f));
    m = m * glm::scale(glm::vec3(1.5f, 1.5f, 1.5f));
    m_shader->setUniform("m", m);
    m_shader->applyMaterial(m_whiteMaterial);
    m_sun->draw();

    m_shader->unbind();
}

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    crepscularRayPass();

    /*
     * This is just a quick note on how to use the camera, we can delete this before we push the final code.
     * Up, down, left, right arrow- "moves" the camera in this direction. The camera itself isn't moved, but its
     * like turning your head in the direction you press on your keyboard.
     *
     * W key- moves the camera 1 unit forwards- this actually moves the camera's position
     * S key- moves the camera 1 unit backwards- also moves the camera
     */
}
