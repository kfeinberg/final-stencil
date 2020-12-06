#include "TexturedShape.h"

#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

using namespace CS123::GL;

using namespace CS123::GL;

TexturedShape::TexturedShape()
{
}

TexturedShape::~TexturedShape()
{
}

void TexturedShape::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

void TexturedShape::buildVAO() {
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
