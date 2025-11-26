#include<iostream>
#include<string>
#include<set>
#include<map>

using State=std::string;   //folosim un alias ca sa fie mai sugestiv
using Symbol=char;
using TransitionFunction=std::map<State,std::map<Symbol,State>>;



class DeterministicFiniteAutomaton{
private:
    std::set<State>Q;  //multimea starilor
    std::set<Symbol>Sigma;  //alfabetul din care este format cuvantul
    TransitionFunction Delta; // functia de tranzitie,inca nu stiu exact ce e
    State initialState; //starea initial
    std::set<State> finalStates; // multimea starilor finale


public:
    DeterministicFiniteAutomaton()=default;
    void setAutomaton(const std::set<State>&states,const std::set<Symbol>&alphabet,const TransitionFunction &delta,const State &initial,const std::set<State>&final){
        Q=states;
        Sigma=alphabet;
        Delta=delta;
        initialState=initial;
        finalStates=final;
    }

    bool VerifyAutomaton()const;
    void PrintAutomaton()const;
    bool CheckWord(const std::string&word);

};