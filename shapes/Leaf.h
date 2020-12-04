#ifndef LEAF_H
#define LEAF_H

#include <memory>
#include <vector>

#include "GL/glew.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

namespace CS123 { namespace GL {
class VAO;
}}

class Leaf
{
public:
    Leaf();
    ~Leaf();

    void draw();

private:
    void buildVAO();

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;
};

#endif // LEAF_H
