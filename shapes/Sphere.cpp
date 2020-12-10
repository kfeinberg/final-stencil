#include "glm/gtx/transform.hpp"  // translate, scale, rotate
#include "glm/gtc/constants.hpp" // useful constants

#include "Sphere.h"

Sphere::Sphere(int shapeParameter1, int shapeParameter2) :
    Shape(),
    m_shapeParameter1(shapeParameter1),
    m_shapeParameter2(shapeParameter2)
{
    m_shapeParameter1 = std::max(m_shapeParameter1, 2);
    m_shapeParameter2 = std::max(m_shapeParameter2, 3);
    generateVertices();
}

Sphere::~Sphere()
{
}

void Sphere::updateParameters(int shapeParameter1, int shapeParameter2, float shapeParameter3) {
    int adjustedParameter1 = std::max(shapeParameter1, 2);
    int adjustedParameter2 = std::max(shapeParameter2, 3);
    if (m_shapeParameter1 != adjustedParameter1 || m_shapeParameter2 != adjustedParameter2) {
        m_shapeParameter1 = adjustedParameter1;
        m_shapeParameter2 = adjustedParameter2;
        generateVertices();
    }
}

void Sphere::generateVertices() {
    int vecsPerTriangle = attributesPerVertex * verticesPerTriangle;
    int vecsPerFace = 2 * vecsPerTriangle + attributesPerVertex * trianglesPerQuad * verticesPerTriangle * (m_shapeParameter1 - 2);
    int totalFloats = floatsPerVec * m_shapeParameter2 * vecsPerFace;

    std::vector<glm::vec4> triangle;
    triangle.reserve(vecsPerTriangle);
    std::vector<glm::vec4> face;
    face.reserve(vecsPerFace);
    m_vertexData.clear();
    m_vertexData.reserve(totalFloats);

    // creating top triangle
    float theta = glm::half_pi<float>();
    float dTheta = 2.0f * glm::pi<float>() / static_cast<float>(m_shapeParameter2);
    float p1 = static_cast<float>(m_shapeParameter1);
    triangle.push_back(glm::vec4(calculateCoordinates(0.0f, theta), 1.0f));
    calculateAndStoreNormal(triangle);
    triangle.push_back(glm::vec4(calculateCoordinates(glm::pi<float>()/p1, theta), 1.0f));
    calculateAndStoreNormal(triangle);
    triangle.push_back(glm::vec4(calculateCoordinates(glm::pi<float>()/p1, theta-dTheta), 1.0f));
    calculateAndStoreNormal(triangle);
    // copy triangle over to face
    for (glm::vec4 &v : triangle) {
        face.push_back(v);
    }
    // use translation to get matching bottom triangle
    glm::mat4 T = glm::rotate(glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
    Shape::applyTransformation(T, triangle);
    // copy triangle over to face
    for (glm::vec4 &v : triangle) {
        face.push_back(v);
    }

    // generating all quads
    for (int layer = 1; layer < m_shapeParameter1-1; layer++) {
        face.push_back(glm::vec4(calculateCoordinates(glm::pi<float>()*layer/p1, theta), 1.0f));
        calculateAndStoreNormal(face);
        face.push_back(glm::vec4(calculateCoordinates(glm::pi<float>()*(layer+1)/p1, theta-dTheta), 1.0f));
        calculateAndStoreNormal(face);
        face.push_back(glm::vec4(calculateCoordinates(glm::pi<float>()*layer/p1, theta-dTheta), 1.0f));
        calculateAndStoreNormal(face);
        face.push_back(glm::vec4(calculateCoordinates(glm::pi<float>()*layer/p1, theta), 1.0f));
        calculateAndStoreNormal(face);
        face.push_back(glm::vec4(calculateCoordinates(glm::pi<float>()*(layer+1)/p1, theta), 1.0f));
        calculateAndStoreNormal(face);
        face.push_back(glm::vec4(calculateCoordinates(glm::pi<float>()*(layer+1)/p1, theta-dTheta), 1.0f));
        calculateAndStoreNormal(face);
    }

    // we rotate our initial face so perspective matches demo
    T = glm::rotate(glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
    Shape::applyTransformation(T, face);

    // creating all faces of sphere
    T = glm::rotate(dTheta, glm::vec3(0.0f, 1.0f, 0.0f));
    Shape::transformAndStore(T, face, m_shapeParameter2);

    buildVAO();
}

glm::vec3 Sphere::calculateCoordinates(float phi, float theta, float r) {
    float x = r * glm::sin(phi) * glm::cos(theta);
    float y = r * glm::cos(phi);
    float z = r * glm::sin(phi) * glm::sin(theta);

    return glm::vec3(x, y, z);
}

void Sphere::calculateAndStoreNormal(std::vector<glm::vec4> &v) {
    glm::vec4 normal = glm::vec4(v.back());
    normal.w = 0.0f; // normals have w set to 0 by convention
    v.push_back(glm::normalize(normal));
}
