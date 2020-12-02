#include "Scene.h"

#include "lib/ResourceLoader.h"
#include "lib/CS123SceneData.h"
#include "shapes/Cylinder.h"
#include <sstream>

#include "gl/GLDebug.h" // useful for debugging shader stuff

using namespace CS123::GL;

Scene::Scene()
{
    // loading and compiling shader
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/shader.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/shader.frag");
    m_shader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

Scene::~Scene()
{
}

Camera *Scene::getCamera() {
    return &m_camera;
}

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    m_shader->setUniform("m", glm::mat4(1.0f));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    tree.draw();
    m_shader->unbind();
}
