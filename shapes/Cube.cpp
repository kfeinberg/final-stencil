#include "glm/gtx/transform.hpp"  // translate, scale, rotate
#include "glm/gtc/constants.hpp" // useful constants

#include "Cube.h"

Cube::Cube(int shapeParameter1) :
    Shape(),
    m_shapeParameter1(shapeParameter1)
{
    m_shapeParameter1 = std::max(m_shapeParameter1, 1);
    generateVertices();
}

Cube::~Cube()
{
}

void Cube::updateParameters(int shapeParameter1, int shapeParameter2, float shapeParameter3) {
    int adjustedParameter1 = std::max(shapeParameter1, 1);
    if (m_shapeParameter1 != adjustedParameter1) {
        m_shapeParameter1 = adjustedParameter1;
        generateVertices();
    }
}

void Cube::generateVertices() {
    int vecsPerFace = attributesPerVertex * verticesPerTriangle * trianglesPerQuad * m_shapeParameter1 * m_shapeParameter1;
    int totalFloats = floatsPerVec * facesPerCube * vecsPerFace;

    std::vector<glm::vec4> face;
    face.reserve(vecsPerFace);
    m_vertexData.clear();
    m_vertexData.reserve(totalFloats);

    // calculate all vertices and normals for positive z face, x and y lie in [0, 1]
    float p1 = static_cast<float>(m_shapeParameter1);
    glm::vec4 normal = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f); // normal vector for positive z face is always the same
    for (int y = 0; y < m_shapeParameter1; y++) {
        for (int x = 0; x < m_shapeParameter1; x++) {
            face.push_back(glm::vec4(x/p1, y/p1, 0.5f, 1.0f));
            face.push_back(normal);
            face.push_back(glm::vec4((x+1)/p1, (y+1)/p1, 0.5f, 1.0f));
            face.push_back(normal);
            face.push_back(glm::vec4(x/p1, (y+1)/p1, 0.5f, 1.0f));
            face.push_back(normal);
            face.push_back(glm::vec4(x/p1, y/p1, 0.5f, 1.0f));
            face.push_back(normal);
            face.push_back(glm::vec4((x+1)/p1, y/p1, 0.5f, 1.0f));
            face.push_back(normal);
            face.push_back(glm::vec4((x+1)/p1, (y+1)/p1, 0.5f, 1.0f));
            face.push_back(normal);
        }
    }

    // we need to translate x and y from [0, 1] to [-0.5, 0.5]
    glm::mat4 T = glm::translate(glm::vec3(-0.5f, -0.5f, 0.0f));
    Shape::applyTransformation(T, face);

    // use 90 degree rotations to get front, top, back, and bottom sides of cube
    T = glm::rotate(glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    Shape::transformAndStore(T, face, 4);

    // left and right sides have diagonals in other direction, we need to translate our initial face
    T = glm::rotate(glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
    Shape::applyTransformation(T, face);

    // now we move the front face to the right side
    T = glm::rotate(glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
    Shape::applyTransformation(T, face);

    // use 180 degree rotations to get the left and right sides of cube
    T = glm::rotate(glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
    Shape::transformAndStore(T, face, 2);

    buildVAO();
}
