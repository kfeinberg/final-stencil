#ifndef GRASS_H
#define GRASS_H

#include <memory>
#include <vector>

#include "GL/glew.h"

namespace CS123 { namespace GL {
class VAO;
}}

class Grass
{
public:
    Grass();
    ~Grass();

    void draw();

private:
    void buildVAO();

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;
};

#endif // GRASS_H
