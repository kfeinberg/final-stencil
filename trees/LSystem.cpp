#include "LSystem.h"

LSystem::LSystem()
{}

LSystem::LSystem(std::map<char, std::vector<std::string>> rules, std::string axiom) {
    m_rules = rules;
    m_axiom = axiom;
}

std::string LSystem::applyRules(int depth) {

    std::string curr = m_axiom; // curr string begins as axiom
    std::string next;

    for (int iter = 0; iter < depth; iter++) { // perform rules depth number of times

        next = ""; // reset "next" string

        for (int i = 0; i < curr.length(); i++) { // iterate through current string

            if (m_rules.count(curr[i]) == 0) { // check if letter has a rule, do nothing if it doesn't
                next = next + curr[i];
            }
            else { // if letter has rule, apply rule
               std::vector<std::string> v = m_rules.at(curr[i]);
               int idx = rand() % (v.size()-1 + 1);
               next = next + v[idx];
            }

        }
        curr = next; // update curr to be next string
    }
    return curr;
}

void LSystem::setRules(std::map<char, std::vector<std::string>> rules) {
    m_rules = rules;
}

void LSystem::setAxiom(std::string axiom) {
    m_axiom = axiom;
}
