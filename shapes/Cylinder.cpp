#include "Cylinder.h"
#include "CylinderComponent.h"
#include "ShapeComponent.h"
#include "GL/glew.h"
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions

Cylinder::Cylinder()
{
}

Cylinder::Cylinder(int param1, int param2) :
    m_param1(param1),
    m_param2(param2)
{
    glm::mat4 rotateMat1 = glm::rotate((float)M_PI/2, glm::vec3(1, 0, 0));
    glm::mat4 transMat1 = glm::translate(glm::vec3(0, -.5, 0));
    CylinderComponent * sc1 = new CylinderComponent(param1, param2, rotateMat1, transMat1, 0.5, CYLINDER);

    glm::mat4 rotateMat2 = glm::rotate((float)M_PI*3/2, glm::vec3(1, 0, 0));
    glm::mat4 transMat2 = glm::translate(glm::vec3(0, 0.5, 0));
    CylinderComponent * sc2 = new CylinderComponent(param1, param2, rotateMat2, transMat2, 0.5, CYLINDER);

    std::vector<float> connection = connectFaces(sc1, sc2, param1, param2);

    // regular coordinates
    m_vertexData.insert(m_vertexData.end(), connection.begin(), connection.end());
    m_vertexData.insert(m_vertexData.end(), sc1->triangles.begin(), sc1->triangles.end());
    m_vertexData.insert(m_vertexData.end(), sc2->triangles.begin(), sc2->triangles.end());

    delete sc1;
    delete sc2;

    buildVAO();
}

Cylinder::~Cylinder()
{
}
