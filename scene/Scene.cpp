#include "Scene.h"

#include <sstream>
#include <QImage>

#include "lib/ResourceLoader.h"
#include "lib/CS123SceneData.h"
#include "shapes/Cylinder.h"
#include "shapes/Grass.h"
#include "gl/textures/TextureParameters.h"
#include "gl/textures/TextureParametersBuilder.h"
#include "trees/LSystem.h"
#include "trees/Turtle.h"

#include "gl/GLDebug.h" // useful for debugging shader stuff

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
}

Scene::~Scene()
{
}

Camera *Scene::getCamera() {
    return &m_camera;
}

void Scene::drawTree() {

    // source: https://github.com/abiusx/L3D/blob/master/L%2B%2B/tree.l%2B%2B
//    std::map<char, std::string> rules;
//    rules['A'] = "^F>(30)B\\B\\\\\B";
//    rules['B'] = "[^^FL\\\\\\AL]";
//    rules['L'] = "[^(60)[*(.3)]+(50)*(.28)]";

//    LSystem l = LSystem(rules, "FA");

//    std::string res = l.applyRules(3);
    std::string res = "F-F";

    Turtle t = Turtle();
    t.parseInput(res);

    Cylinder branch(20, 20);

    for (glm::mat4x4 mat: t.m_cylinderTransformations) {
         m_shader->setUniform("m", mat);
         branch.draw();
    }
}

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // setting camera orientation
    m_camera.orientLook(
                glm::vec4(2.f, 2.f, 2.f, 1.f),
                glm::vec4(-1.f, -1.f, -1.f, 0.f),
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
