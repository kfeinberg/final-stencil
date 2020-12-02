#ifndef SCENE_H
#define SCENE_H

#include "gl/shaders/CS123Shader.h"
#include <memory>

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

private:
    std::unique_ptr<CS123::GL::CS123Shader> m_shader;
};

#endif // SCENE_H
