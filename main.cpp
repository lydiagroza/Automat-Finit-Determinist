#include <iostream>
#include "DeterministicFiniteAutomaton.h"
#include "RegexToDFA.h"

int main()
{
    DeterministicFiniteAutomaton dfa;
    std::string word;
    int choice;

    std::cout << "For CheckWord enter 1, for RegexToDFA enter 2: " << std::endl;
    std::cin >> choice;

    if (choice == 1)
    {
        std::set<State> Q = {"q0", "q1", "q2", "q3"};
        std::set<Symbol> Sigma = {'a', 'b'};
        TransitionFunction Delta;

        std::cout << "Tabelul de tranzitii al automatului: " << std::endl;

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

        if (!dfa.VerifyAutomaton()) {
            std::cerr << "Automatul nu este valid!" << std::endl;
            return 1;
        }

        dfa.PrintAutomaton();

        while (word != "exit") {
            std::cout << "Enter word: ";
            std::cin >> word;

            if (word == "exit") {
                break;
            }

            if (dfa.CheckWord(word)) {
                std::cout << "Cuvantul este acceptat de automat." << std::endl;
            } else {
                std::cout << "Cuvantul nu este acceptat de automat." << std::endl;
            }
        }
    }
    else if (choice == 2) {
        std::string regex;
        std::cout << "Enter Regex (examples: (a|b)*, ab*a, a(b|c)*d): ";
        std::cin >> regex;

        std::set<State> states;
        std::set<Symbol> alphabet;
        TransitionFunction delta;
        State initialState;
        std::set<State> finalStates;

        if (RegexToDFA::convertRegexToDFA(regex, states, alphabet, delta, initialState, finalStates)) {
            std::cout << "\nDFA successfully created!" << std::endl;

            dfa.setAutomaton(states, alphabet, delta, initialState, finalStates);

            if (!dfa.VerifyAutomaton()) {
                std::cerr << "Generated DFA is not valid!" << std::endl;
                return 1;
            }

            std::cout << "\nDFA Transition Table:" << std::endl;
            dfa.PrintAutomaton();

            std::cout << "\nInitial state: " << initialState << std::endl;
            std::cout << "Accept states: ";
            for (const auto& fs : finalStates) {
                std::cout << fs << " ";
            }
            std::cout << std::endl;

            // Test words
            word = "";
            while (word != "exit") {
                std::cout << "\nEnter word to test (or 'exit' to quit): ";
                std::cin >> word;

                if (word == "exit") {
                    break;
                }

                if (dfa.CheckWord(word)) {
                    std::cout << "✓ Word '" << word << "' is ACCEPTED by the automaton." << std::endl;
                } else {
                    std::cout << "✗ Word '" << word << "' is REJECTED by the automaton." << std::endl;
                }
            }
        } else {
            std::cerr << "Failed to convert regex to DFA!" << std::endl;
            return 1;
        }
    }

    return 0;
}