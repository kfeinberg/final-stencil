#include "Scene.h"

using namespace CS123::GL;

Scene::Scene()
{
    // creating primitives
    m_ground = std::make_unique<Plane>();
    m_grass = std::make_unique<Grass>();

    // loading and compiling shader
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/shader.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/shader.frag");
    m_shader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

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
