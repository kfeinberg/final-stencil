#include "Scene.h"

using namespace CS123::GL;

Scene::Scene()
{
    // creating primitives
    m_ground = std::make_unique<Plane>();
    m_grass = std::make_unique<Grass>();
    m_branch = std::make_unique<Cylinder>(1, 8);
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

    // loading bark texture
    QImage barkImage(":/images/bark.jpg");
    m_barkTexture = std::make_unique<Texture2D>(barkImage.bits(), barkImage.width(), barkImage.height());
    builder.setFilter(TextureParameters::FILTER_METHOD::NEAREST);
    builder.setWrap(TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE);
    params = builder.build();
    params.applyTo(*m_barkTexture.get());

    // setting camera orientation
    m_camera.orientLook(
                glm::vec4(0.f, 2.f, 0.f, 1.f), // eye position
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
    m_woodMaterial.cDiffuse = m_woodMaterial.cAmbient;
    m_woodMaterial.cAmbient *= 0.1f;

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

    // set sky material
    m_skyMaterial.clear();
    m_skyMaterial.cAmbient.r = 135.f / 255.f;
    m_skyMaterial.cAmbient.g = 206.f / 255.f;
    m_skyMaterial.cAmbient.b = 250.f / 255.f;

    // initialize quad used for crepscular ray display
    std::vector<GLfloat> quadData;
    quadData = {-1, 1, 0, 0, 1, -1, -1, 0, 0, 0, 1, 1, 0, 1, 1, 1, -1, 0, 1, 0};
    m_quad = std::make_unique<OpenGLShape>();
    m_quad->setVertexData(&quadData[0], quadData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_quad->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->setAttribute(ShaderAttrib::TEXCOORD0, 2, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->buildVAO();

    initializeTreePositions();
    initializeGrassPositions();

    initializeTrees();

    addLighting();
}

Scene::~Scene()
{
}

/**
 * @brief Initialize trees in m_trees to use throughout scene.
 */
void Scene::initializeTrees() {
    // source: https://github.com/abiusx/L3D/blob/master/L%2B%2B/tree.l%2B%2B
    std::map<char, std::vector<std::string>> rules;

    rules['A']=std::vector<std::string>{"^FB//B*//*///B"};
    rules['B']=std::vector<std::string>{"[>^*^F*//*//*//A]", "[>&F*/*/*//*//A]"};

    std::string axiom = "FA";

    for (int i = 0; i < 5; i++) {
        Tree t = Tree(rules, axiom, 9);
        m_trees.push_back(t);
    }
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
    m_crepscularRayShader->setUniform("sunPos", glm::vec3(0.f, .8f, -1.f)); // TODO: update with real sun pos

    glViewport(0, 0, m_width, m_height);
    m_quad->draw(); // renders combined image with crespcular rays

    m_crepscularRayShader->unbind();
}

void Scene::initializeTreePositions() {

    // right section
//    for (int x = 10; x <= 19; x+=3) {
//        for (int z = -18; z <= 18; z+=3) {
//            float x_coord = scatterPoint(x, 1.4f);
//            float z_coord = scatterPoint(z, 1.4f);
//            m_treeTrans.push_back(glm::translate(glm::vec3(x_coord, 0.f, z_coord)));
//        }
//    }

    // left section
//    for (int x = -19; x <= -10; x+=3) {
//        for (int z = -18; z <= 18; z+=3) {
//            float x_coord = scatterPoint(x, 1.4f);
//            float z_coord = scatterPoint(z, 1.4f);
//            m_treeTrans.push_back(glm::translate(glm::vec3(x_coord, 0.f, z_coord)));
//        }
//    }

    // top section
    for (int x = -10; x <= 10; x+=3) {
        for (int z = -18; z <= -10; z+=3) {
            float x_coord = scatterPoint(x, 1.4f);
            float z_coord = scatterPoint(z, 1.4f);
            m_treeTrans.push_back(glm::translate(glm::vec3(x_coord, 0.f, z_coord)));
        }
    }

    // bottom section
//    for (int x = -10; x <= 10; x+=3) {
//        for (int z = 10; z <= 18; z+=3) {
//            float x_coord = scatterPoint(x, 1.4f);
//            float z_coord = scatterPoint(z, 1.4f);
//            m_treeTrans.push_back(glm::translate(glm::vec3(x_coord, 0.f, z_coord)));
//        }
//    }
}

void Scene::initializeGrassPositions() {
    float step = 0.4f;
    for (float x = -18; x <= 18; x+=step) {
        for (float z = -18; z <= 18; z+=step) {

            // carpet grass with semi randomness
            float x_coord = scatterPoint(x, .12f);
            float z_coord = scatterPoint(z, .12f);
            glm::mat4x4 m = glm::translate(glm::vec3(x_coord, 0.5f, z_coord));

            // random theta for random rotation
            float theta = (((float) rand()) / (float) RAND_MAX) * M_2_PI;
            if (theta != 0.0f) {
                m = m * glm::rotate(theta, glm::vec3(0, 1, 0));
            }

            // random scale for higher/lower grass
            float scale_val = scatterPoint(.8f, .2f);
            m = m * glm::scale(glm::vec3(scale_val, scale_val, .8f));

            m_grassTrans.push_back(m);
        }
    }
}

void Scene::renderPrimitives(bool occluded) {

    for (int i = 0; i < m_treeTrans.size(); i++) {
        treePass(occluded, i%5, m_treeTrans[i]);
    }

    sunPass();
    groundPass(occluded);
}

void Scene::groundPass(bool occluded) {
    m_shader->bind();
    m_shader->setUniform("useLighting", true);
    m_shader->setUniform("useTexture", false);
    m_shader->setUniform("p", m_camera.getProjectionMatrix());
    m_shader->setUniform("v", m_camera.getViewMatrix());

    // ground
    glm::mat4 m;
    m = m * glm::rotate(glm::half_pi<float>(), glm::vec3(1.f, 0.f, 0.f));
    m = m * glm::scale(glm::vec3(skyBoxDim, skyBoxDim, 0.f));
    m_shader->setUniform("m", m);
    if (occluded) {
         m_shader->applyMaterial(m_occludedMaterial);
    }
    else {
        m_shader->applyMaterial(m_leafMaterial);
    }
    m_ground->draw();

    // top
    m = glm::translate(glm::vec3(0.f, skyBoxDim/2.f, 0.f));
    m = m * glm::rotate(glm::half_pi<float>(), glm::vec3(1.f, 0.f, 0.f));
    m = m * glm::scale(glm::vec3(skyBoxDim, skyBoxDim, 0.f));
    m_shader->setUniform("m", m);
    if (occluded) {
         m_shader->applyMaterial(m_occludedMaterial);
    }
    else {
        m_shader->applyMaterial(m_skyMaterial);
    }
    m_ground->draw();

    // side
    m = glm::translate(glm::vec3(0.f, skyBoxDim/2.f, -skyBoxDim/2.f));
    m = m * glm::scale(glm::vec3(skyBoxDim, skyBoxDim, 0.f));
    m_shader->setUniform("m", m);
    if (occluded) {
         m_shader->applyMaterial(m_occludedMaterial);
    }
    else {
        m_shader->applyMaterial(m_skyMaterial);
    }
    m_ground->draw();

    // side
    m = glm::translate(glm::vec3(0.f, skyBoxDim/2.f, skyBoxDim/2.f));
    m = m * glm::scale(glm::vec3(skyBoxDim, skyBoxDim, 0.f));
    m_shader->setUniform("m", m);
    if (occluded) {
         m_shader->applyMaterial(m_occludedMaterial);
    }
    else {
        m_shader->applyMaterial(m_skyMaterial);
    }
    m_ground->draw();

    // side
    m = glm::translate(glm::vec3(skyBoxDim/2.f, skyBoxDim/2.f, 0.f));
    m = m * glm::rotate(glm::half_pi<float>(), glm::vec3(0.f, 1.f, 0.f));
    m = m * glm::scale(glm::vec3(skyBoxDim, skyBoxDim, 0.f));
    m_shader->setUniform("m", m);
    if (occluded) {
         m_shader->applyMaterial(m_occludedMaterial);
    }
    else {
        m_shader->applyMaterial(m_skyMaterial);
    }
    m_ground->draw();

    // side
    m = glm::translate(glm::vec3(-skyBoxDim/2.f, skyBoxDim/2.f, 0.f));
    m = m * glm::rotate(glm::half_pi<float>(), glm::vec3(0.f, 1.f, 0.f));
    m = m * glm::scale(glm::vec3(skyBoxDim, skyBoxDim, 0.f));
    m_shader->setUniform("m", m);
    if (occluded) {
         m_shader->applyMaterial(m_occludedMaterial);
    }
    else {
        m_shader->applyMaterial(m_skyMaterial);
    }
    m_ground->draw();

    m_shader->unbind();
}

void Scene::grassPass(bool occluded, glm::mat4x4 trans) {
    m_shader->bind();
    m_grassTexture->bind();
    m_shader->setUniform("useLighting", true);
    m_shader->setUniform("p", m_camera.getProjectionMatrix());
    m_shader->setUniform("v", m_camera.getViewMatrix());


    m_shader->setUniform("m", trans);
    m_shader->setUniform("useTexture", true);

    if (occluded) {
        m_shader->applyMaterial(m_occludedMaterial);
        m_shader->setUniform("isOccluded", true);
    }
    else {
        m_shader->applyMaterial(m_leafMaterial);
        m_shader->setUniform("isOccluded", false);
    }

    m_grass->draw();
    m_grassTexture->unbind();
    m_shader->unbind();
}

// Source: https://www.gamasutra.com/view/feature/130071/random_scattering_creating_.php?page=2
/**
 * @brief CreateScatter randomly scatters curr to have value curr-scatter < ret < curr+scatter
 * @param cur current point value.
 * @param scatter amount to scatter curr by
 * @return new scattered value
 */
float Scene::scatterPoint(float curr, float scatter) {
    float r = (((float) rand()) / (float) RAND_MAX) * 2 * scatter + -scatter;
    return curr + r;
}

/**
 * @brief Scene::treePass draws one tree in the scene.
 * @param occluded: if the tree should be occluded or not
 * @param trans: translation of the tree
 * @param t: integer for which tree to draw
 */
void Scene::treePass(bool occluded, int t, glm::mat4x4 trans) {
    m_shader->bind();
    m_shader->setUniform("useLighting", true);
    m_shader->setUniform("useTexture", false);
    m_shader->setUniform("p", m_camera.getProjectionMatrix());
    m_shader->setUniform("v", m_camera.getViewMatrix());
    //m_barkTexture->bind();

    Tree curr = m_trees[t];

    std::vector<glm::mat4x4> cyl_trans = curr.getTranformations();
    std::vector<TreeComponents> comps = curr.getComponents();

    for (size_t i = 0; i < cyl_trans.size(); i++) {
        m_shader->setUniform("m", trans * cyl_trans[i]);
        if (comps[i] == TreeComponents::BRANCH) {
            if (occluded) {
                 m_shader->applyMaterial(m_occludedMaterial);
            }
            else {
                m_shader->applyMaterial(m_woodMaterial);
                //m_shader->setUniform("useTexture", true);
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
    //m_barkTexture->unbind();
    m_shader->unbind();
}

void Scene::sunPass() {
    m_shader->bind();

    m_shader->setUniform("p", m_camera.getProjectionMatrix());
    m_shader->setUniform("v", m_camera.getViewMatrix());
    m_shader->setUniform("useLighting", true);
    m_shader->setUniform("useTexture", false);

    glm::mat4 m;
    m = m * glm::translate(glm::vec3(0.f, 7.f, -22.f));
    m = m * glm::scale(glm::vec3(4.0f, 4.0f, 4.0f));
    m_shader->setUniform("m", m);
    m_shader->applyMaterial(m_whiteMaterial);
    m_sun->draw();

    m_shader->unbind();
}

void Scene::addLighting() {
    m_shader->bind();
    m_shader->setUniform("useLighting", true);
    int i = 0;

    CS123SceneLightData l;
    l.type = LightType::LIGHT_POINT;
    l.pos = glm::vec4(0.f, 7.f, -22.f, 1.f);
    l.color = glm::vec4(1.f, 1.f, 1.f, 1.f);
    l.id = i;
    m_shader->setLight(l);
    i++;

    l.type = LightType::LIGHT_DIRECTIONAL;
    l.dir = glm::normalize(-glm::vec4(0.f, 7.f, -22.f, 0.f));
    l.color = glm::vec4(1.f, 1.f, 1.f, 1.f);
    l.id = i;
    m_shader->setLight(l);
    i++;

    m_shader->unbind();
}

/*
 * This is just a quick note on how to use the camera, we can delete this before we push the final code.
 * Up, down, left, right arrow- "moves" the camera in this direction. The camera itself isn't moved, but its
 * like turning your head in the direction you press on your keyboard.
 *
 * W key- moves the camera 1 unit forwards- this actually moves the camera's position
 * S key- moves the camera 1 unit backwards- also moves the camera
 */
void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //renderPrimitives(false); // renders all primitives without crepuscular rays
    crepscularRayPass();
//    for (glm::mat4x4 trans: m_grassTrans) {
//        grassPass(false, trans);
//    }
}
