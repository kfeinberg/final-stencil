#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <map>
#include <string>

class LSystem
{
public:
    LSystem();
    LSystem(std::map<char, std::string>, std::string);
    std::string applyRules(int depth);
    void setRules(std::map<char, std::string>);
    void setAxiom(std::string);
private:
    std::map<char, std::string> m_rules; // map of rules
    std::string m_axiom; // starting word
};

#endif // LSYSTEM_H
