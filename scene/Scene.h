#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "gl/shaders/CS123Shader.h"
#include "gl/shaders/Shader.h"
#include "gl/textures/Texture2D.h"
#include "camera/CamtransCamera.h"

#include <sstream>
#include <QImage>

#include "lib/ResourceLoader.h"
#include "lib/CS123SceneData.h"
#include "shapes/Cylinder.h"
#include "shapes/Grass.h"
#include "shapes/Plane.h"
#include "shapes/Leaf.h"
#include "shapes/CylinderComponent.h"
#include "gl/textures/TextureParameters.h"
#include "gl/textures/TextureParametersBuilder.h"
#include "trees/LSystem.h"
#include "trees/Turtle.h"

#include "gl/GLDebug.h" // useful for debugging shader stuff

namespace CS123 { namespace GL {
    class Shader;
    class CS123Shader;
    class Texture2D;
}}

class Scene
{
public:
    Scene();
    ~Scene();

    void render();
    void drawTree();
    Camera *getCamera();

private:
    void groundPass();
    void grassPass();

    std::unique_ptr<CS123::GL::CS123Shader> m_shader;
    std::unique_ptr<CS123::GL::Texture2D> m_grassTexture;
    CamtransCamera m_camera;
    CS123SceneMaterial m_woodMaterial;
    CS123SceneMaterial m_leafMaterial;
    CS123SceneMaterial m_occludedMaterial; // black material used for making occluded scene
    std::unique_ptr<TexturedShape> m_ground;
    std::unique_ptr<TexturedShape> m_grass;
};

#endif // SCENE_H
