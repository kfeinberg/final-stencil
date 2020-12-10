#include "glm/gtc/constants.hpp" // useful constants

#include "CircularBaseShape.h"

CircularBaseShape::CircularBaseShape(int shapeParameter1, int shapeParameter2) :
    Shape(),
    m_shapeParameter1(shapeParameter1),
    m_shapeParameter2(shapeParameter2)
{
    m_shapeParameter1 = std::max(m_shapeParameter1, 1);
    m_shapeParameter2 = std::max(m_shapeParameter2, 3);
}

CircularBaseShape::~CircularBaseShape()
{
}

void CircularBaseShape::updateParameters(int shapeParameter1, int shapeParameter2, float shapeParameter3) {
    int adjustedParameter1 = std::max(shapeParameter1, 1);
    int adjustedParameter2 = std::max(shapeParameter2, 3);
    if (m_shapeParameter1 != adjustedParameter1 || m_shapeParameter2 != adjustedParameter2) {
        m_shapeParameter1 = adjustedParameter1;
        m_shapeParameter2 = adjustedParameter2;
        generateVertices();
    }
}

void CircularBaseShape::generateCircularBaseSlice(std::vector<glm::vec4> &v) {
    float dTheta = 2.0f * glm::pi<float>() / static_cast<float>(m_shapeParameter2);
    float theta = glm::half_pi<float>() - dTheta;
    float x = 0.5f * glm::cos(theta);
    float z = 0.5f * glm::sin(theta);
    float p1 = static_cast<float>(m_shapeParameter1);

    glm::vec4 normal = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

    // creating tiny triangle at top
    v.push_back(glm::vec4(0.0f, -0.5f, 0.0f, 1.0f));
    v.push_back(normal);
    v.push_back(glm::vec4(1.0f/p1*x, -0.5f, 1.0f/p1*z, 1.0f));
    v.push_back(normal);
    v.push_back(glm::vec4(0.0f, -0.5f, 1.0f/p1*0.5f, 1.0f));
    v.push_back(normal);

    // creating the rest of the layers
    for (int layer = 1; layer < m_shapeParameter1; layer++) {
        v.push_back(glm::vec4(0.0f, -0.5f, layer/p1*0.5f, 1.0f));
        v.push_back(normal);
        v.push_back(glm::vec4(layer/p1*x, -0.5f, layer/p1*z, 1.0f));
        v.push_back(normal);
        v.push_back(glm::vec4((layer+1)/p1*x, -0.5f, (layer+1)/p1*z, 1.0f));
        v.push_back(normal);
        v.push_back(glm::vec4(0.0f, -0.5f, layer/p1*0.5f, 1.0f));
        v.push_back(normal);
        v.push_back(glm::vec4((layer+1)/p1*x, -0.5f, (layer+1)/p1*z, 1.0f));
        v.push_back(normal);
        v.push_back(glm::vec4(0.0f, -0.5f, (layer+1)/p1*0.5f, 1.0f));
        v.push_back(normal);
    }
}
