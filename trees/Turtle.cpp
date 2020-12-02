#include "Turtle.h"

Turtle::Turtle()
{

}

/**
 * @brief The LSystem class
 *
 * Symbolic explanations:
 * F move forward by d units
 * + yaw left by angle δ
 * - yaw right by angle δ
 * & pitch down by angle δ
 * ∧ pitch up by angle δ
 * \ roll left by angle δ
 * / roll right by angle δ
 * [ start of new branch (push the state onto the stack)
 * ] end of branch (pop the state from the stack and old state becomes current state)
 * > : decrease thickness (by percent of its own)
 * < : increase thickness (by percent of its own)
 * = : set thickness (to percentage of length)
 * * : draw leaf
 */


/**
 * @brief Iterates through string and performs actions.
 * @param input string generated by L-system.
 */
void Turtle::parseInput(std::string input) {
    for (std::string::size_type i = 0; i < input.size(); i++) {
        char c = input[i];
        switch(c) {
            case 'F':
                moveForward();
                break;
            case '+':
                m_yaw += theta;
                break;
            case '-':
                m_yaw -= theta;
                break;
            case '&':
                m_pitch += theta;
                break;
            case '^':
                m_pitch -= theta;
                break;
            case '\\':
                m_roll += theta;
                break;
            case '/':
                m_roll -= theta;
                break;
            case '[':
                this->save();
                break;
            case ']':
                this->restore();
                break;
            case '>':
                m_thickness *= dec_mult;
                break;
            case '<':
                m_thickness *= inc_mult;
                break;
            case '=':
                m_thickness = 1;
                break;
            case '*':
                drawLeaf();
                break;
        }
    }
}

/**
 * @brief Draw cylinder and move turtle forward.
 */
void Turtle::moveForward() {

    // draw branch segment
    Cylinder c(10, 10); //TODO: transform cylinder to turtle pos/rot
    c.draw();

    // calculate forward vector based on yaw, pitch, deg
    // source: https://stackoverflow.com/questions/1568568/how-to-convert-euler-angles-to-directional-vector
    float fx = cos(m_yaw) * cos(m_pitch);
    float fy = sin(m_yaw) * cos(m_pitch);
    float fz = sin(m_pitch);

    // update position vector with forward step
    glm::vec3 vec = glm::normalize(glm::vec3(fx, fy, fz));
    m_pos += vec;
}

/**
 * @brief Draws leaf at current turtle location.
 */
void Turtle::drawLeaf() {
    Cylinder c(10, 10); //TODO: Find better leaf shape, transform to pos
    c.draw();
}

/**
 * @brief Save current turtle state on stack.
 */
void Turtle::save() {
    m_states.push(*this);
}

/**
 * @brief End branch by restoring previous turtle state from stack.
 */
void Turtle::restore() {
    Turtle t = m_states.top();
    m_states.pop();

    m_pos = t.m_pos;
    m_thickness = t.m_thickness;
    m_pitch = t.m_pitch;
    m_yaw = t.m_yaw;
    m_roll = t.m_roll;
}