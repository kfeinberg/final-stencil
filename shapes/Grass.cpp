#include "Grass.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

Grass::Grass()
{
    std::vector<glm::vec4> positions;
    glm::vec4 normal = glm::vec4(0.f, 1.f, 0.f, 0.f);
    std::vector<glm::vec2> uvs;

    positions.push_back(glm::vec4(-.5f, -.5f, 0.f, 1.f));
    positions.push_back(glm::vec4(.5f, .5f, 0.f, 1.f));
    positions.push_back(glm::vec4(-.5f, .5f, 0.f, 1.f));
    positions.push_back(glm::vec4(-.5f, -.5f, 0.f, 1.f));
    positions.push_back(glm::vec4(.5f, -.5f, 0.f, 1.f));
    positions.push_back(glm::vec4(.5f, .5f, 0.f, 1.f));

    uvs.push_back(glm::vec2(0.f, 1.0f));
    uvs.push_back(glm::vec2(1.f, 0.0f));
    uvs.push_back(glm::vec2(0.f, 0.0f));
    uvs.push_back(glm::vec2(0.f, 1.0f));
    uvs.push_back(glm::vec2(1.f, 1.0f));
    uvs.push_back(glm::vec2(1.f, 0.0f));

    for (size_t i = 0; i < positions.size(); i++) {
        insertVec(m_vertexData, positions[i]);
        insertVec(m_vertexData, normal);
        insertVec(m_vertexData, uvs[i]);
    }

    float angle = glm::two_thirds<float>() * glm::pi<float>();
    glm::mat4 rotate = glm::rotate(angle, glm::vec3(0.f, 1.f, 0.f));

    for (glm::vec4 &position : positions) {
        position = rotate * position;
    }
    for (size_t i = 0; i < positions.size(); i++) {
        insertVec(m_vertexData, positions[i]);
        insertVec(m_vertexData, normal);
        insertVec(m_vertexData, uvs[i]);
    }

    for (glm::vec4 &position : positions) {
        position = rotate * position;
    }
    for (size_t i = 0; i < positions.size(); i++) {
        insertVec(m_vertexData, positions[i]);
        insertVec(m_vertexData, normal);
        insertVec(m_vertexData, uvs[i]);
    }

    buildVAO();
}

Grass::~Grass()
{
}
