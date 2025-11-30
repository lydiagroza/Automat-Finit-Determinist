#include<iostream>
#include<string>
#include<set>
#include<map>

using State=std::string;
using Symbol=char;
using TransitionFunction=std::map<State,std::map<Symbol,State>>;



class DeterministicFiniteAutomaton{
private:
    std::set<State>Q;
    std::set<Symbol>Sigma;
    TransitionFunction Delta;
    State initialState;
    std::set<State> finalStates;


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