#include "Scene.h"

using namespace CS123::GL;

Scene::Scene()
{
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

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // setting camera orientation
//    m_camera.orientLook(
//                glm::vec4(2.f, 2.f, 2.f, 1.f),
//                glm::vec4(-1.f, -1.f, -1.f, 0.f),
//                glm::vec4(0.f, 1.f, 0.f, 0.f));

    m_camera.orientLook(
                glm::vec4(0.f, 0.f, 20.f, 1.f),
                glm::vec4(0.f, 0.f, -1.f, 0.f),
                glm::vec4(0.f, 1.f, 0.f, 0.f));

    // creating wood material
    CS123SceneMaterial wood;
    wood.clear();
    wood.cAmbient.r = 133.f / 255.f;
    wood.cAmbient.g = 94.f / 255.f;
    wood.cAmbient.b = 66.f / 255.f;

    // creating cylinder
    Cylinder tree(20, 20);

    m_shader->bind();
    m_shader->setUniform("useLighting", false); // use only ambient and diffuse color
    m_shader->setUniform("useArrowOffsets", false); // skip arrow billboarding
    m_shader->applyMaterial(wood);
    m_shader->setUniform("p", m_camera.getProjectionMatrix());
    m_shader->setUniform("v", m_camera.getViewMatrix());
    m_shader->setUniform("m", glm::mat4(1.0f)); // set for each cylinder
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //tree.draw();

    //Leaf l;
    //l.draw();

    drawTree();

    m_shader->unbind();

    // grass primitive
//    Grass grass;

//    m_shader->bind();
//    m_grassTexture->bind();

//    // sending camera matrices to shader
//    m_shader->setUniform("p", m_camera.getProjectionMatrix());
//    m_shader->setUniform("v", m_camera.getViewMatrix());
//    m_shader->setUniform("m", glm::mat4(1.0f));

//    // setting other shader settings
//    m_shader->setUniform("useLighting", false); // use only ambient and diffuse color
//    m_shader->setUniform("useArrowOffsets", false); // skip arrow billboarding

//    // texturing settings
//    m_shader->setUniform("useTexture", true);

//    // sending primitive color
//    m_shader->applyMaterial(wood);

//    grass.draw();

//    m_grassTexture->unbind();
//    m_shader->unbind();
}
