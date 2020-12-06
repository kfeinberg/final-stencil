#ifndef TEXTUREDSHAPE_H
#define TEXTUREDSHAPE_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "GL/glew.h"

namespace CS123 { namespace GL {
class VAO;
}}

inline void insertVec(std::vector<GLfloat> &dest, const glm::vec4 &target) {
    dest.push_back(target.x);
    dest.push_back(target.y);
    dest.push_back(target.z);
}

inline void insertVec(std::vector<GLfloat> &dest, const glm::vec3 &target) {
    dest.push_back(target.x);
    dest.push_back(target.y);
    dest.push_back(target.z);
}

inline void insertVec(std::vector<GLfloat> &dest, const glm::vec2 &target) {
    dest.push_back(target.x);
    dest.push_back(target.y);
}

class TexturedShape
{
public:
    TexturedShape();
    ~TexturedShape();

    void draw();

protected:
    void buildVAO();

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;
};

#endif // TEXTUREDSHAPE_H
