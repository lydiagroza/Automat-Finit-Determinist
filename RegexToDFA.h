//
// Created by lydia on 11/29/2025.
//

#ifndef AUTOMAT_FINIT_DETERMINIST_REGEXTODFA_H
#define AUTOMAT_FINIT_DETERMINIST_REGEXTODFA_H

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>

using State = std::string;
using Symbol = char;
using TransitionFunction = std::map<State, std::map<Symbol, State>>;

struct NFA
{
    std::set<State> states;
    std::set<Symbol> symbols;
    std::map<State, std::map<Symbol, std::set<State>>> transitions; // doar in AFN
    State startState;
};

class RegexToDFA
{
private:
    static int stateCounter;

    static std::string getNewState()
    {
        return "q" + std::to_string(stateCounter++);
    }

    bool isOperator(char c);
    int Priority(char op);

    static std::string addConcatOperator(const std::string& regex);

    static std::string regexToPostfix(const std::string& regex);

    static NFA postfixNFA(const std::string& postfix);

    static std::set<State> lambdaClosure(const std::set<State>& states, std::map<State, std::map<Symbol, std::set<State>>>& transitions);

    static std::set<State> Move(NFA);
    static void NFAtoDFA();



public:


};


#endif //AUTOMAT_FINIT_DETERMINIST_REGEXTODFA_H