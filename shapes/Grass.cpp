#include "Grass.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

using namespace CS123::GL;

inline void insertVec4(std::vector<GLfloat> &dest, const glm::vec4 &target) {
    dest.push_back(target.x);
    dest.push_back(target.y);
    dest.push_back(target.z);
}

inline void insertVec2(std::vector<GLfloat> &dest, const glm::vec2 &target) {
    dest.push_back(target.x);
    dest.push_back(target.y);
}

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
        insertVec4(m_vertexData, positions[i]);
        insertVec4(m_vertexData, normal);
        insertVec2(m_vertexData, uvs[i]);
    }

    float angle = glm::two_thirds<float>() * glm::pi<float>();
    glm::mat4 rotate = glm::rotate(angle, glm::vec3(0.f, 1.f, 0.f));

    for (glm::vec4 &position : positions) {
        position = rotate * position;
    }
    for (size_t i = 0; i < positions.size(); i++) {
        insertVec4(m_vertexData, positions[i]);
        insertVec4(m_vertexData, normal);
        insertVec2(m_vertexData, uvs[i]);
    }

    for (glm::vec4 &position : positions) {
        position = rotate * position;
    }
    for (size_t i = 0; i < positions.size(); i++) {
        insertVec4(m_vertexData, positions[i]);
        insertVec4(m_vertexData, normal);
        insertVec2(m_vertexData, uvs[i]);
    }

    buildVAO();
}

Grass::~Grass()
{
}

void Grass::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

void Grass::buildVAO() {
    const int numFloatsPerVertex = 8; // 3 for position, 3 for normal, 2 for UV coords
    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.reserve(3);
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    markers.push_back(VBOAttribMarker(ShaderAttrib::TEXCOORD0, 2, 6*sizeof(float)));

    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
}
