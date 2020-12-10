#ifndef SHAPE_H
#define SHAPE_H

/** imports the OpenGL math library https://glm.g-truc.net/0.9.2/api/a00001.html */
#include <glm/glm.hpp>
#include "GL/glew.h"

#include <memory>
#include <vector>

/**
 *
 * inserts a glm::vec3 into a vector of floats
 * this will come in handy if you want to take advantage of vectors to build your shape
 * make sure to call reserve beforehand to speed this up
 */
inline void insertVec3(std::vector<float> &data, glm::vec3 v){
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

static constexpr int trianglesPerQuad = 2;
static constexpr int verticesPerTriangle = 3;
static constexpr int attributesPerVertex = 2;
static constexpr int floatsPerVec = 3;
static constexpr int facesPerCube = 6;

namespace CS123 { namespace GL {
class VAO;
}}

class Shape
{
public:
    Shape();
    virtual ~Shape();

    void draw();
    virtual void updateParameters(int shapeParameter1, int shapeParameter2, float shapeParameter3) = 0;

protected:
    void buildVAO();
    virtual void generateVertices() = 0;
    void applyTransformation(const glm::mat4 &T, std::vector<glm::vec4> &data);
    void storeVertices(std::vector<glm::vec4> &data);
    void transformAndStore(const glm::mat4 &T, std::vector<glm::vec4> &data, int iterations);

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;
};

#endif // SHAPE_H
