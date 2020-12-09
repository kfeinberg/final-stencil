#ifndef TREE_H
#define TREE_H

#include "trees/LSystem.h"
#include "trees/Turtle.h"
#include "shapes/Cylinder.h"
#include "shapes/Leaf.h"


class Tree
{
public:
    Tree(std::map<char, std::vector<std::string>> rules, std::string axiom, int depth);
    std::vector<TreeComponents> getComponents();
    std::vector<glm::mat4x4>getTranformations();
    void setTreeTransformation(glm::mat4x4);
private:
    Turtle m_turtle;
    LSystem m_lsys;
    std::string m_res;
};

#endif // TREE_H
