#include "Leaf.h"

using namespace CS123::GL;

inline void insertVec3(std::vector<GLfloat> &dest, const glm::vec3 &target) {
    dest.push_back(target.x);
    dest.push_back(target.y);
    dest.push_back(target.z);
}

Leaf::Leaf() {

    int sides = 12;
    glm::mat4x4 scale = glm::scale(glm::vec3(1, 2.5f, 1));

    std::vector<float> vertices;

    float divideFactor = 1/static_cast<float>(sides);
    for (int i = 0; i <= sides; i++) {
        vertices.push_back(0.5*cos(i * divideFactor*2*M_PI));
        vertices.push_back(0.5*sin(i * divideFactor*2*M_PI));
        vertices.push_back(0.5*0);
    }

    glm::vec3 normal = glm::vec3(0, 0, 1);

    for (int i = 0; i < sides; i++) {

        int v1 = i;
        int v2 = i+1;

        glm::vec3 bottomLeft = (scale * glm::vec4(vertices[v1*3], vertices[v1*3 + 1], vertices[v1*3 + 2], 1.0f)).xyz();
        glm::vec3 bottomRight = (scale * glm::vec4(vertices[v2*3], vertices[v2*3 + 1], vertices[v2*3 + 2], 1.0f)).xyz();
        glm::vec3 center = (scale * glm::vec4(0, 0, 0, 1.0f)).xyz();

        insertVec3(m_vertexData, bottomLeft);
        insertVec3(m_vertexData, normal);
        insertVec3(m_vertexData, bottomRight);
        insertVec3(m_vertexData, normal);
        insertVec3(m_vertexData, center);
        insertVec3(m_vertexData, normal);
    }

    buildVAO();
}

Leaf::~Leaf()
{
}

void Leaf::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

void Leaf::buildVAO() {
    const int numFloatsPerVertex = 6; // 3 for position, 3 for normal, 2 for UV coords
    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.reserve(3);
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    //markers.push_back(VBOAttribMarker(ShaderAttrib::TEXCOORD0, 2, 6*sizeof(float)));

    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
}
