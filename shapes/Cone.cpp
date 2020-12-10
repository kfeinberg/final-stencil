#include "glm/gtx/transform.hpp"  // translate, scale, rotate
#include "glm/gtc/constants.hpp" // useful constants

#include "Cone.h"

Cone::Cone(int shapeParamter1, int shapeParameter2) :
    CircularBaseShape(shapeParamter1, shapeParameter2)
{
    generateVertices();
}

Cone::~Cone()
{
}

void Cone::generateVertices() {
    int vecsPerFace = attributesPerVertex * (verticesPerTriangle + trianglesPerQuad * verticesPerTriangle * (m_shapeParameter1-1));
    int vecsPerBase = attributesPerVertex * (verticesPerTriangle + trianglesPerQuad * verticesPerTriangle * (m_shapeParameter1-1));
    int totalFloats = floatsPerVec * m_shapeParameter2 * (vecsPerFace + vecsPerBase);

    std::vector<glm::vec4> face;
    face.reserve(vecsPerFace);
    std::vector<glm::vec4> base;
    base.reserve(vecsPerBase);
    m_vertexData.clear();
    m_vertexData.reserve(totalFloats);

    // creating cone face
    float dTheta = 2.0f * glm::pi<float>() / static_cast<float>(m_shapeParameter2);
    float theta = glm::half_pi<float>() - dTheta;
    float rootFive = glm::sqrt(5.0f);
    float x = 0.5f * glm::cos(theta);
    float z = 0.5f * glm::sin(theta);
    glm::mat4 T = glm::rotate(dTheta, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec4 lNorm = glm::normalize(glm::vec4(0.0f, 1.0f/rootFive, 2.0f/rootFive, 0.0f));
    glm::vec4 rNorm = glm::normalize(T*lNorm);
    float p1 = static_cast<float>(m_shapeParameter1);

    // creating top triangle
    face.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    face.push_back(glm::normalize(lNorm+rNorm));
    face.push_back(glm::vec4(0.0f, 1.0f-1.0f/p1, 1.0f/p1*0.5f, 1.0f));
    face.push_back(lNorm);
    face.push_back(glm::vec4(1.0f/p1*x, 1.0f-1.0f/p1, 1.0f/p1*z, 1.0f));
    face.push_back(rNorm);

    // creating rest of face
    for (int layer = 1; layer < m_shapeParameter1; layer++) {
        face.push_back(glm::vec4(0.0f, 1.0f-layer/p1, 0.5f*layer/p1, 1.0f));
        face.push_back(lNorm);
        face.push_back(glm::vec4(0.0f, 1.0f-(layer+1)/p1, 0.5f*(layer+1)/p1, 1.0f));
        face.push_back(lNorm);
        face.push_back(glm::vec4(x*layer/p1, 1.0f-layer/p1, z*layer/p1, 1.0f));
        face.push_back(rNorm);
        face.push_back(glm::vec4(0.0f, 1.0f-(layer+1)/p1, 0.5f*(layer+1)/p1, 1.0f));
        face.push_back(lNorm);
        face.push_back(glm::vec4(x*(layer+1)/p1, 1.0f-(layer+1)/p1, z*(layer+1)/p1, 1.0f));
        face.push_back(rNorm);
        face.push_back(glm::vec4(x*layer/p1, 1.0f-layer/p1, z*layer/p1, 1.0f));
        face.push_back(rNorm);
    }

    // moving face back into y [-0.5, 0.5]
    T = glm::translate(glm::vec3(0.0f, -0.5f, 0.0f));
    Shape::applyTransformation(T, face);

    // generating all sides of cone
    T = glm::rotate(dTheta, glm::vec3(0.0f, 1.0f, 0.0f));
    Shape::transformAndStore(T, face, m_shapeParameter2);

    // creating bottom base
    CircularBaseShape::generateCircularBaseSlice(base);
    Shape::transformAndStore(T, base, m_shapeParameter2);

    buildVAO();
}
