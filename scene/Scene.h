#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "gl/shaders/CS123Shader.h"
#include "gl/shaders/Shader.h"
#include "gl/textures/Texture2D.h"
#include "camera/CamtransCamera.h"

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

public:
    void render();
    void drawTree();
    Camera *getCamera();

private:
    std::unique_ptr<CS123::GL::CS123Shader> m_shader;
    std::unique_ptr<CS123::GL::Texture2D> m_grassTexture;
    CamtransCamera m_camera;
};

#endif // SCENE_H
