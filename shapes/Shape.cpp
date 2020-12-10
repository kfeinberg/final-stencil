#include "Shape.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"

using namespace CS123::GL;

Shape::Shape() :
    m_VAO(nullptr)
{

}

Shape::~Shape()
{
}

void Shape::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

void Shape::buildVAO() {    
    const int numFloatsPerVertex = 6; // 3 for position, 3 for normal, 2 for UV coords
    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.reserve(3);
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
}

void Shape::applyTransformation(const glm::mat4 &T, std::vector<glm::vec4> &data) {
    for (glm::vec4 &v : data) {
        v = T * v;
    }
}

void Shape::storeVertices(std::vector<glm::vec4> &data) {
    for (const glm::vec4 &v : data) {
        m_vertexData.push_back(v.x);
        m_vertexData.push_back(v.y);
        m_vertexData.push_back(v.z);
    }
}

void Shape::transformAndStore(const glm::mat4 &T, std::vector<glm::vec4> &data, int iterations) {
    for (int i = 0; i < iterations; i++) {
        storeVertices(data);
        applyTransformation(T, data);
    }
}
