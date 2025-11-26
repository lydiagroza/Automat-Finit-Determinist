#include<iostream>
#include "DeterministicFiniteAutomaton.h"

int main()
{
    DeterministicFiniteAutomaton dfa;
    std::set<State> Q = {"q0", "q1"};

    std::set<Symbol> Sigma = {'a', 'b'};

    TransitionFunction Delta;
    //aaa

    Delta["q0"]['a'] = "q1";
    Delta["q0"]['b'] = "q0";


    Delta["q1"]['a'] = "q2";
    Delta["q1"]['b'] = "q1";


    Delta["q2"]['a'] = "q3";
    Delta["q2"]['b'] = "q1";


    Delta["q3"]['a'] = "q0";
    Delta["q3"]['b'] = "q1";


    State initialState = "q0";


    std::set<State> finalStates = {"q3"};

    dfa.setAutomaton(Q, Sigma, Delta, initialState, finalStates);
    dfa.PrintAutomaton();
    if (dfa.CheckWord("aca") == true) {
        std::cout << "Cuvantul este acceptat de automat." << std::endl;
    } else {
        std::cout << "Cuvantul nu este acceptat de automat." << std::endl;
    }


    return 0;
}