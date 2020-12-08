#include "Tree.h"

Tree::Tree(std::map<char, std::string> rules, std::string axiom, int depth) {
    m_lsys = LSystem(rules, axiom);

    std::string res = m_lsys.applyRules(depth);
    m_turtle.parseInput(res);
}

std::vector<TreeComponents> Tree::getComponents() {
    return m_turtle.m_treeComponents;
}
std::vector<glm::mat4x4> Tree::getTranformations() {
    return m_turtle.m_cylinderTransformations;
}
