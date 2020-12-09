#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <map>
#include <string>
#include <vector>

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

class LSystem
{
public:
    LSystem();
    LSystem(std::map<char, std::vector<std::string>>, std::string);
    std::string applyRules(int depth);
    void setRules(std::map<char, std::vector<std::string>>);
    void setAxiom(std::string);
private:
    std::map<char, std::vector<std::string>> m_rules; // map of rules
    std::string m_axiom; // starting word

    // TODO: add non-deterministic  rules?
};

#endif // LSYSTEM_H
