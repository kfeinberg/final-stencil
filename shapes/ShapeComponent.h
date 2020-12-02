#ifndef SHAPECOMPONENT_H
#define SHAPECOMPONENT_H

#include <iostream>

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "GL/glew.h"

#include <vector>

class ShapeComponent
{
public:
    ShapeComponent();
    ShapeComponent(int rows, int cols, glm::mat4 rotation, glm::mat4 translation);
    ShapeComponent(glm::vec3 bottomLeft,  glm::vec3 topLeft, glm::vec3 bottomRight, glm::vec3 topRight,
                   int rows, int cols, std::vector<glm::vec3> normals, glm::mat4 rotation, glm::mat4 translation);
    ShapeComponent(glm::vec3 bottomLeft,  glm::vec3 topLeft, glm::vec3 bottomRight, glm::vec3 topRight,
                   int rows, int cols, std::vector<glm::vec3> normals);
    ShapeComponent(glm::vec3 bottomLeft,  glm::vec3 topLeft, glm::vec3 bottomRight, glm::vec3 topRight,
                   int rows, int cols, int shape);
    ~ShapeComponent();
    std::vector<float>triangles;
    std::vector<float> getTriangles();

private:
    glm::vec3 m_bottom_left;
    glm::vec3 m_top_right;
    glm::vec3 m_bottom_right;
    glm::vec3 m_top_left;

    int m_rows;
    int m_cols;

    std::vector<glm::vec3>m_normals;
    std::vector<float>vertices;
    glm::mat4 rotationMatrix;
    glm::mat4 translationMatrix;

    void makeVertices();
    void makeTriangles();
    void makeTrianglesNoRotate();
    void addToTrianglesVector(std::vector<glm::vec3> points, std::vector<glm::vec3> normals);
};

#endif // SHAPECOMPONENT_H
