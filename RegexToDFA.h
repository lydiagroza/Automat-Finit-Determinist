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
    std::set<State> finalStates;

};


class RegexToDFA
{
private:
    static int stateCounter;

    static std::string getNewState()
    {
        return "q" + std::to_string(stateCounter++);
    }

    static int Priority(char op);

    static std::string addConcatOperator(const std::string& regex);

    static NFA postfixNFA(const std::string& postfix);

    static std::set<State> lambdaClosure(const std::set<State>& states,
                                          const std::map<State, std::map<Symbol, std::set<State>>>& transitions);

    static std::set<State> Move(const NFA& nfa, const std::set<State>& states, Symbol symbol);

    static void NFATODFA(const NFA& nfa,
                          std::set<State>& dfaStates,
                          std::set<Symbol>& dfaAlphabet,
                          TransitionFunction& dfaTransitions,
                          State& dfaStartState,
                          std::set<State>& dfaAcceptStates);

    static std::string setToString(const std::set<State>& stateSet);

public:
    static std::string getPostfix(const std::string& regex);
    static std::string getSyntaxTree(const std::string& regex);
    static std::string regexToPostfix(const std::string& regex);

    static bool convertRegexToDFA(const std::string& regex,
                                   std::set<State>& states,
                                   std::set<Symbol>& alphabet,
                                   TransitionFunction& delta,
                                   State& initialState,
                                   std::set<State>& finalStates);

    static void printDFA(const std::set<State>& states,
                        const std::set<Symbol>& alphabet,
                        const TransitionFunction& delta,
                        const State& initialState,
                        const std::set<State>& finalStates);

};


#endif //AUTOMAT_FINIT_DETERMINIST_REGEXTODFA_H