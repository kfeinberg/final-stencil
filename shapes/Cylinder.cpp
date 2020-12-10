#include "glm/gtx/transform.hpp"  // translate, scale, rotate
#include "glm/gtc/constants.hpp" // useful constants

#include "Cylinder.h"

Cylinder::Cylinder(int shapeParameter1, int shapeParameter2) :
    CircularBaseShape(shapeParameter1, shapeParameter2)
{
    generateVertices();
}

Cylinder::~Cylinder()
{
}

void Cylinder::generateVertices() {
    int vecsPerFace = attributesPerVertex * trianglesPerQuad * verticesPerTriangle * m_shapeParameter1;
    int vecsPerBase = attributesPerVertex * (verticesPerTriangle + trianglesPerQuad * verticesPerTriangle * (m_shapeParameter1-1));
    int totalFloats = floatsPerVec * m_shapeParameter2 * (vecsPerFace + 2 * vecsPerBase);

    std::vector<glm::vec4> face;
    face.reserve(vecsPerFace);
    std::vector<glm::vec4> base;
    base.reserve(vecsPerBase);
    m_vertexData.clear();
    m_vertexData.reserve(totalFloats);

    // creating barrel face
    float dTheta = 2.0f * glm::pi<float>() / static_cast<float>(m_shapeParameter2);
    float theta = glm::half_pi<float>() - dTheta;
    float x = 0.5f * glm::cos(theta);
    float z = 0.5f * glm::sin(theta);
    float normX = glm::cos(theta);
    float normZ = glm::sin(theta);
    float p1 = static_cast<float>(m_shapeParameter1);
    glm::vec4 lNorm = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    glm::vec4 rNorm = glm::vec4(normX, 0.0f, normZ, 0.0f);

    for (int y = 0; y < m_shapeParameter1; y++) {
        face.push_back(glm::vec4(0.0f, y/p1, 0.5f, 1.0f));
        face.push_back(lNorm);
        face.push_back(glm::vec4(x, (y+1)/p1, z, 1.0f));
        face.push_back(rNorm);
        face.push_back(glm::vec4(0.0f, (y+1)/p1, 0.5f, 1.0f));
        face.push_back(lNorm);
        face.push_back(glm::vec4(0.0f, y/p1, 0.5f, 1.0f));
        face.push_back(lNorm);
        face.push_back(glm::vec4(x, y/p1, z, 1.0f));
        face.push_back(rNorm);
        face.push_back(glm::vec4(x, (y+1)/p1, z, 1.0f));
        face.push_back(rNorm);
    }

    // moving y back into the range [-0.5, 0.5]
    glm::mat4 T = glm::translate(glm::vec3(0.0f, -0.5f, 0.0f));
    Shape::applyTransformation(T, face);

    // generating all sides of cylinder barrel
    T = glm::rotate(dTheta, glm::vec3(0.0f, 1.0f, 0.0f));
    Shape::transformAndStore(T, face, m_shapeParameter2);

    // creating base slice
    CircularBaseShape::generateCircularBaseSlice(base);

    // generating entire bottom base
    Shape::transformAndStore(T, base, m_shapeParameter2);

    // we need to flip the bottom base so it faces up
    T = glm::rotate(glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
    Shape::applyTransformation(T, base);

    // creating entire top base
    T = glm::rotate(dTheta, glm::vec3(0.0f, 1.0f, 0.0f));
    Shape::transformAndStore(T, base, m_shapeParameter2);

    buildVAO();
}
