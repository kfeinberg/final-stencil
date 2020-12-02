#include "Shape.h"
#include "ShapeComponent.h"
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
    const int numFloatsPerVertex = 6;
    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));

    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
}

std::vector<float> Shape::connectFaces(std::vector<float> vertices1, std::vector<float> vertices2,
                                           std::vector<glm::vec3>sideNormals, int param1, int param2, glm::mat4 combinedMat1, glm::mat4 combinedMat2) {

    std::vector<float> connection;
    for (int i = 0; i < std::max(3, param2); i++) {

        glm::vec3 normal1 = sideNormals[i];
        glm::vec3 normal2 = sideNormals[i+1];

        glm::vec3 v1 = (combinedMat1*glm::vec4(vertices1[i*3], vertices1[i*3 + 1], vertices1[i*3 + 2], 1.0f)).xyz();
        //bottom left
        glm::vec3 v2 = (combinedMat1*glm::vec4(vertices1[(i+1)*3], vertices1[(i+1)*3 + 1], vertices1[(i+1)*3 + 2], 1.0f)).xyz();
        //top left
        glm::vec3 v3 = (combinedMat2*glm::vec4(vertices2[i*3], vertices2[i*3 + 1], vertices2[i*3 + 2], 1.0f)).xyz();
        //bottom right
        glm::vec3 v4 = (combinedMat2*glm::vec4(vertices2[(i+1)*3], vertices2[(i+1)*3 + 1], vertices2[(i+1)*3 + 2], 1.0f)).xyz();
        //top right

        ShapeComponent * sc = new ShapeComponent(v1, v2, v3, v4, 1, param1, std::vector<glm::vec3>{(combinedMat1*glm::vec4(normal1, 0.0)).xyz(),
                                                                                                   (combinedMat1*glm::vec4(normal2, 0.0)).xyz(), (combinedMat1*glm::vec4(normal1, 0.0)).xyz(), (combinedMat1*glm::vec4(normal2,0.0)).xyz()});
        connection.insert(connection.end(), sc->triangles.begin(), sc->triangles.end());
    }
    return connection;
}

std::vector<float> Shape::connectFaces(CylinderComponent * c1, CylinderComponent * c2, int param1, int param2) {

    std::vector<float> connection;
    for (int i = 0; i < std::max(3, param2); i++) {

        glm::vec3 normal1 = c1->sideNormals[i];
        glm::vec3 normal2 = c1->sideNormals[i+1];

        glm::vec3 normal3 = c2->sideNormals[i];
        glm::vec3 normal4 = c2->sideNormals[i+1];

        glm::vec3 v1 = (c1->translationMatrix*c1->rotationMatrix*glm::vec4(c1->vertices[i*3], c1->vertices[i*3 + 1], c1->vertices[i*3 + 2], 1.0f)).xyz();
        //bottom left
        glm::vec3 v2 = (c1->translationMatrix*c1->rotationMatrix*glm::vec4(c1->vertices[(i+1)*3], c1->vertices[(i+1)*3 + 1], c1->vertices[(i+1)*3 + 2], 1.0f)).xyz();
        //top left

        glm::vec3 v3 = (c2->translationMatrix*c1->rotationMatrix*glm::vec4(c2->vertices[i*3], c2->vertices[i*3 + 1], c2->vertices[i*3 + 2], 1.0f)).xyz();
        //bottom right
        glm::vec3 v4 = (c2->translationMatrix*c1->rotationMatrix*glm::vec4(c2->vertices[(i+1)*3], c2->vertices[(i+1)*3 + 1], c2->vertices[(i+1)*3 + 2], 1.0f)).xyz();
        //top right

        ShapeComponent * sc = new ShapeComponent(v1, v2, v3, v4, 1, param1, std::vector<glm::vec3>{normal1, normal2, normal3, normal4});

        connection.insert(connection.end(), sc->triangles.begin(), sc->triangles.end());
    }
    return connection;
}
