#include<iostream>
#include "DeterministicFiniteAutomaton.h"

int main()
{
    DeterministicFiniteAutomaton dfa;
    std::set<State> Q = {"q0", "q1"};

    std::set<Symbol> Sigma = {'a', 'b'};

    TransitionFunction Delta;

    Delta["q0"]['a'] = "q1";
    Delta["q0"]['b'] = "q0";


    Delta["q1"]['a'] = "q0";
    Delta["q1"]['b'] = "q1";


    Delta["q2"]['a'] = "q0";
    Delta["q2"]['b'] = "q1";


    Delta["q3"]['a'] = "q0";
    Delta["q3"]['b'] = "q1";


    State initialState = "q0";


    std::set<State> finalStates = {"q0"};

    dfa.setAutomaton(Q, Sigma, Delta, initialState, finalStates);
    dfa.PrintAutomaton();


    return 0;
}