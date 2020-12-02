#ifndef SHAPE_H
#define SHAPE_H

/** imports the OpenGL math library https://glm.g-truc.net/0.9.2/api/a00001.html */
#include <glm/glm.hpp>
#include "GL/glew.h"
#include "CylinderComponent.h"

#include<memory>
#include <vector>

/**
 *
 * inserts a glm::vec3 into a vector of floats
 * this will come in handy if you want to take advantage of vectors to build your shape
 * make sure to call reserve beforehand to speed this up
 */
void insertVec3(std::vector<float> &data, glm::vec3 v);

namespace CS123 { namespace GL {
class VAO;
}}

class Shape
{
public:
    Shape();
    virtual ~Shape();
    void draw();
    std::vector<float> connectFaces(std::vector<float> vertices1, std::vector<float> vertices2,
                                        std::vector<glm::vec3>sideNormals, int param1, int param2, glm::mat4 combinedMat1, glm::mat4 combinedMat2);
    std::vector<float> connectFaces(CylinderComponent * c1, CylinderComponent * c2, int param1, int param2);
    std::vector<GLfloat> m_vertexData;
    glm::vec2 calculateUVCylinderBody(glm::vec3 inter);
    glm::vec2 calculateUVSphere(glm::vec3 inter);

protected:
    /** builds the VAO, pretty much the same as from lab 1 */
    void buildVAO();

    std::unique_ptr<CS123::GL::VAO> m_VAO;
};

#endif // SHAPE_H
