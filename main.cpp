#include <iostream>
#include <fstream>
#include "DeterministicFiniteAutomaton.h"
#include "RegexToDFA.h"

void printMenu() {
    std::cout << "\nMenu:\n";
    std::cout << "1. Afisarea formei poloneze postfixate a expresiei regulate\n";
    std::cout << "2. Afisarea arborelui sintactic corespunzator expresiei regulate\n";
    std::cout << "3. Afisarea automatului M atat in consola, cat si intr-un fisier de iesire\n";
    std::cout << "4. Verificarea unuia sau mai multor cuvinte in automatul M\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    std::ifstream regexFile("../regex.txt");
    if (!regexFile.is_open()) {
        std::cerr << "Error: Could not open regex.txt at '../regex.txt'\n";
        return 1;
    }

    std::string regex;
    std::getline(regexFile, regex);
    regexFile.close();

    std::cout << "Read Regular Expression from file: '" << regex << "'" << std::endl;
    if (regex.empty()) {
        std::cerr << "Error: Regular expression read from file is empty.\n";
        return 1;
    }

    DeterministicFiniteAutomaton dfa;
    std::set<State> states;
    std::set<Symbol> alphabet;
    TransitionFunction delta;
    State initialState;
    std::set<State> finalStates;

    std::cout << "Attempting to convert Regex to DFA..." << std::endl;
    if (!RegexToDFA::convertRegexToDFA(regex, states, alphabet, delta, initialState, finalStates)) {
        std::cerr << "Failed to convert regex to DFA! (RegexToDFA::convertRegexToDFA returned false)" << std::endl;
        return 1;
    }
    std::cout << "Regex to DFA conversion successful." << std::endl;

    dfa.setAutomaton(states, alphabet, delta, initialState, finalStates);

    std::cout << "Verifying generated DFA..." << std::endl;
    if (!dfa.VerifyAutomaton()) {
        std::cerr << "Generated DFA is not valid! (dfa.VerifyAutomaton returned false)" << std::endl;
        return 1;
    }
    std::cout << "DFA verification successful." << std::endl;

    int choice;
    do {
        printMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string postfix = RegexToDFA::getPostfix(regex);
                std::cout << "Forma poloneza postfixata: " << postfix << std::endl;
                break;
            }
            case 2: {
                std::cout << "Arborele sintactic:\n";
                RegexToDFA::printSyntaxTreeGraph(regex);
                std::cout << std::endl;
                break;
            }
            case 3: {
                dfa.PrintAutomaton();
                std::ofstream outFile("./dfa.txt");
                if (outFile.is_open()) {
                    outFile << "Stari: ";
                    for (const auto& state : states) outFile << state << " ";
                    outFile << "\n";

                    outFile << "Alfabet: ";
                    for (const auto& symbol : alphabet) outFile << symbol << " ";
                    outFile << "\n";

                    outFile << "Tranzitii:\n";
                    for (const auto& transition : delta) {
                        for (const auto& trans : transition.second) {
                            outFile << transition.first << " --" << trans.first << "--> " << trans.second << "\n";
                        }
                    }

                    outFile << "Stare initiala: " << initialState << "\n";
                    
                    outFile << "Stari finale: ";
                    for (const auto& state : finalStates) outFile << state << " ";
                    outFile << "\n";

                    outFile.close();
                    std::cout << "DFA also written to ./dfa.txt\n";
                } else {
                    std::cerr << "Could not open dfa.txt for writing in current directory.\n";
                }
                break;
            }
            case 4: {
                std::string word;
                std::cout << "Enter word to check (or 'exit' to stop): ";
                std::cin >> word;
                while (word != "exit") {
                    if (dfa.CheckWord(word)) {
                        std::cout << "Cuvantul este acceptat de automat.\n";
                    } else {
                        std::cout << "Cuvantul nu este acceptat de automat.\n";
                    }
                    std::cout << "Enter word to check (or 'exit' to stop): ";
                    std::cin >> word;
                }

                break;
            }
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
