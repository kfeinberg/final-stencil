#ifndef SCENE_H
#define SCENE_H

#include "gl/shaders/CS123Shader.h"
#include <memory>
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
    Camera *getCamera();

private:
    std::unique_ptr<CS123::GL::CS123Shader> m_shader;
    CamtransCamera m_camera;
};

#endif // SCENE_H
