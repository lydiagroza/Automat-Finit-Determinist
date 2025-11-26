#include "DeterministicFiniteAutomaton.h"

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
    std::cout<<"   ";
    for (auto it=Sigma.begin();it!=Sigma.end();++it){
        std::cout<<"| "<<*it<<"  ";
    }
    std::cout<<std::endl;
    std::cout<<"---|----|---";
    std::cout<<std::endl;

    for (const auto& state_entry : Delta)
    {
        std::cout<<state_entry.first<<" ";
        for (const auto& transition : state_entry.second)
        {
            std::cout<<"| "<<transition.second<<" ";

        }
        std::cout<<std::endl;
    }
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