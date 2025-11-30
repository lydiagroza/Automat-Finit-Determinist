#include "DeterministicFiniteAutomaton.h"
#include <vector>
#include <algorithm>

bool DeterministicFiniteAutomaton::VerifyAutomaton() const
{
    if(Q.find(initialState)==Q.end()){
        return false;
    }
    for(auto it=finalStates.begin();it!=finalStates.end();++it){
        if(Q.find(*it)==Q.end()){
            return false;
        }
    }

    for (const auto& state_entry : Delta) {
        const State& from_state = state_entry.first; // Starea Sursă


        if (Q.find(from_state) == Q.end()) {
            std::cerr << "Eroare: Starea sursă '" << from_state << "' din Delta nu este în Q." << std::endl;
            return false;
        }

        for (const auto& transition : state_entry.second) {
            const Symbol& symbol = transition.first;    // Simbolul citit
            const State& to_state = transition.second;  // Starea Destinație

            if (Sigma.find(symbol) == Sigma.end()) {
                std::cerr << "Eroare: Simbolul '" << symbol << "' din Delta nu este în alfabetul Σ." << std::endl;
                return false;
            }

            if (Q.find(to_state) == Q.end()) {
                std::cerr << "Eroare: Starea destinație '" << to_state << "' din Delta nu este în Q." << std::endl;
                return false;
            }
        }
    }
    return true;
}

void DeterministicFiniteAutomaton::PrintAutomaton() const
{
    std::cout << "\n+--------------------------------------------------------+" << std::endl;
    std::cout << "|              DFA TRANSITION TABLE                      |" << std::endl;
    std::cout << "+--------------------------------------------------------+\n" << std::endl;

    // Determine the maximum width needed for state names
    size_t maxStateLen = 0;
    for (const auto& state : Q) {
        if (state.length() > maxStateLen) {
            maxStateLen = state.length();
        }
    }
    // Also consider the length of the header "State"
    if (maxStateLen < 5) maxStateLen = 5;

    // Set a reasonable maximum to prevent excessively wide tables
    const size_t MAX_DISPLAY_WIDTH = 40;
    size_t displayStateLen = (maxStateLen > MAX_DISPLAY_WIDTH) ? MAX_DISPLAY_WIDTH : maxStateLen;


    // Print header with alphabet symbols
    std::cout << "State";
    for (size_t i = 5; i < displayStateLen; i++) {
        std::cout << " ";
    }
    std::cout << "  ";

    // Create a sorted list of symbols
    std::vector<Symbol> sortedSigma(Sigma.begin(), Sigma.end());
    std::sort(sortedSigma.begin(), sortedSigma.end());

    // Determine width for transition columns
    size_t transitionWidth = 0;
    for (const auto& state : Q) {
        for (const auto& symbol : sortedSigma) {
            if (Delta.count(state) && Delta.at(state).count(symbol)) {
                if (Delta.at(state).at(symbol).length() > transitionWidth) {
                    transitionWidth = Delta.at(state).at(symbol).length();
                }
            }
        }
    }
    if (transitionWidth < 3) transitionWidth = 3; // for "---"
    if (transitionWidth > MAX_DISPLAY_WIDTH) transitionWidth = MAX_DISPLAY_WIDTH;


    for (const auto& symbol : sortedSigma) {
        std::cout << "| " << symbol;
        for (size_t i = 1; i < transitionWidth; i++) std::cout << " ";
        std::cout << " ";
    }
    std::cout << std::endl;

    // Print separator line
    for (size_t i = 0; i < displayStateLen + 2; i++) {
        std::cout << "-";
    }
    for (size_t i = 0; i < sortedSigma.size(); i++) {
        std::cout << "+";
        for (size_t j = 0; j < transitionWidth + 2; j++) std::cout << "-";
    }
    std::cout << std::endl;

    // Create a sorted list of states
    std::vector<State> sortedQ(Q.begin(), Q.end());
    std::sort(sortedQ.begin(), sortedQ.end());

    // Print each state and its transitions
    for (const auto& state : sortedQ) {
        bool isInitial = (state == initialState);
        bool isFinal = (finalStates.count(state) > 0);

        std::string marker = "  ";
        if (isInitial && isFinal) marker = "->*";
        else if (isInitial) marker = "-> ";
        else if (isFinal) marker = " *";

        std::cout << marker;

        // Print state name, truncating if necessary
        std::string stateName = state;
        if (stateName.length() > displayStateLen) {
            stateName = stateName.substr(0, displayStateLen - 3) + "...";
        }
        std::cout << stateName;

        // Pad to align columns
        for (size_t i = stateName.length(); i < displayStateLen; i++) {
            std::cout << " ";
        }

        // Print transitions for each symbol
        for (const auto& symbol : sortedSigma) {
            std::cout << "| ";

            if (Delta.count(state) && Delta.at(state).count(symbol)) {
                std::string nextState = Delta.at(state).at(symbol);
                if (nextState.length() > transitionWidth) {
                    nextState = nextState.substr(0, transitionWidth - 3) + "...";
                }
                std::cout << nextState;
                for (size_t i = nextState.length(); i < transitionWidth; i++) std::cout << " ";

            } else {
                std::cout << "---";
                 for (size_t i = 3; i < transitionWidth; i++) std::cout << " ";
            }
             std::cout << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n" << std::endl;

    // Print legend
    std::cout << "Legend:" << std::endl;
    std::cout << "  -> : Initial state" << std::endl;
    std::cout << "  * : Accept/Final state" << std::endl;
    std::cout << "  ->*: Initial and Accept state" << std::endl;
    std::cout << "  ---: No transition defined" << std::endl;

    // Print statistics
    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "DFA Statistics:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "  Total states: " << Q.size() << std::endl;
    std::cout << "  Alphabet size: " << Sigma.size() << std::endl;
    std::cout << "  Alphabet: { ";
    bool firstSym = true;
    for (const auto& sym : sortedSigma) {
        if (!firstSym) std::cout << ", ";
        std::cout << sym;
        firstSym = false;
    }
    std::cout << " }" << std::endl;
    std::cout << "  Initial state: " << initialState << std::endl;
    std::cout << "  Accept states (" << finalStates.size() << "): ";

    bool firstFinal = true;
    for (const auto& fs : finalStates) {
        if (!firstFinal) std::cout << ", ";
        std::cout << fs;
        firstFinal = false;
    }
    std::cout << std::endl;
    std::cout << "----------------------------------------\n" << std::endl;
}

bool DeterministicFiniteAutomaton::CheckWord(const std::string& word) {

    State currentState = initialState;
    char c = word[0];
    if (Delta[currentState].find(c) == Delta[currentState].end()) {
        return false;
    }
    currentState = Delta[currentState][c];
    for (int i = 1; i < word.length(); i++) {
        if (Delta[currentState].find(word[i]) == Delta[currentState].end()) {
            return false;
        }
        currentState = Delta[currentState][word[i]];
    }
    if (finalStates.count(currentState) == 0) {
        return false;
    }
    return true;
}