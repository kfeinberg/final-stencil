#ifndef TURTLE_H
#define TURTLE_H

#include<string>
#include<iostream>
#include<stack>

#include <glm.hpp> // glm functions
#include "shapes/Cylinder.h"
#include "shapes/Shape.h"

const float theta = 30;
const float inc_mult = 2.0;
const float dec_mult = 0.5;

class Turtle
{
public:
    Turtle();
    void parseInput(std::string input);
    void moveForward();
    void drawLeaf();
    void restore(); // finished branch, return to previous branch
    void save(); // save current position and start new branch

    // TODO: public for testing, possibly change to private
    std::stack<Turtle>m_states;
    glm::vec3 m_pos; // current position
    float m_thickness; // current thickness of branch
    float m_pitch; // degree of pitch (rotation around x), looking down z-axis
    float m_yaw; // degree of yaw (rotation around y)
    float m_roll; // degree of roll (rotation aruond z)
};

#endif // TURTLE_H
