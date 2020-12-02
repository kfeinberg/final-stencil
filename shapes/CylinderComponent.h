#ifndef CylinderComponent_H
#define CylinderComponent_H

#include <iostream>

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "GL/glew.h"

#include <vector>

// Enumeration values for the Shapes that the user can select in the GUI.
enum ShapeEnum {
    CONE,
    SPHERE,
    CYLINDER,
    TORUS,
};

class CylinderComponent
{
public:
    CylinderComponent();
    CylinderComponent(int param1, int param2, glm::mat4 rotation, glm::mat4 translation, float radius, ShapeEnum shape);
    CylinderComponent(int param1, int param2, glm::mat4 rotation, glm::mat4 translation, float radius, ShapeEnum shape, float phi);
    ~CylinderComponent();

    std::vector<float>triangles;
    std::vector<float>vertices;
    std::vector<glm::vec3> sideNormals;
    glm::mat4 rotationMatrix;
    glm::mat4 translationMatrix;

private:
    int complexity;
    int sides;

    float m_phi;
    float m_radius;
    ShapeEnum m_shape;

    glm::vec3 bottom_left = glm::vec3(-0.5f, -0.5f, 0.0f);
    glm::vec3 top_left = glm::vec3(-0.5f, 0.5f, 0.0f);
    glm::vec3 top_right = glm::vec3(0.5f, 0.5f, 0.0f);
    glm::vec3 bottom_right = glm::vec3(0.5f, -0.5f, 0.0f);
    void makeVertices();
    void makeTriangles();
    void addToTrianglesVector(std::vector<glm::vec3> points, glm::vec3 normal);
};

#endif // CylinderComponent_H
