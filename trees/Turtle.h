#ifndef TURTLE_H
#define TURTLE_H

#include<string>
#include<iostream>
#include<stack>

#include <glm.hpp> // glm functions
#include "shapes/Cylinder.h"
#include "shapes/Shape.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// TODO: constants for now, change to parameters
const float theta = 15;
const float f_dist = 1.0f;
const float thickness_mult = 36.0f; // 5% change in thickness
const float length_thickness_ratio = 0.3f; // thickness of branch relative to length
const float leaf_size = .12f; // leaf size
const float leaf_angle = M_PI/9; // angle of leaf relative to branch

enum TreeComponents {
    BRANCH,
    LEAF
};

/**
 * citations:
 * https://github.com/abiusx/L3D (used for inspiration)
 */
class Turtle
{
public:
    Turtle();
    void parseInput(std::string input);
    void executeCommand(char c, bool hasParam, float param);
    void moveForward(); // moves turtle forward, draws branch
    void drawLeaf(); // draws leaf at current location
    void restore(); // finished branch, return to previous branch
    void save(); // save current position and start new branch
    glm::mat4x4 currTransMatrix(); // object space matrix from curr turtle pos/yaw/roll
    glm::vec3 rotate(glm::vec3 vec, glm::vec3 axis, float theta);
    void yawRight(float theta);
    void rollRight(float theta);
    void pitchUp(float theta);
    void resetTurtle(); // resets turtle back to start

    std::vector<glm::mat4x4> m_cylinderTransformations; // transformations applied
    std::vector<TreeComponents> m_treeComponents; // prim to apply transformation to

private:
    std::stack<Turtle>m_states; // retains past state, used for branches
    glm::vec3 m_pos; // current position
    glm::vec3 m_dir; // current direction vector
    glm::vec3 m_right; // current "right" vector
    float m_thickness; // current thickness of branch
};

#endif // TURTLE_H
