
#include "RegexToDFA.h"

#include <stack>

#include <vector>

#include <algorithm>

#include <sstream>



int RegexToDFA::stateCounter = 0;



// Adds explicit concatenation operators ('.') to the regex string

std::string RegexToDFA::addConcatOperator(const std::string& regex) {

    std::string result;

    for (size_t i = 0; i < regex.length(); i++) {

        result += regex[i];

        if (i + 1 < regex.length()) {

            char curr = regex[i];

            char next = regex[i + 1];

            if ((isalnum(curr) || curr == '*' || curr == ')' || curr == '+') && (isalnum(next) || next == '(')) {

                result += '.';

            }

        }

    }

    return result;

}



// Determines operator precedence for postfix conversion

int RegexToDFA::Priority(char op) {

    switch (op) {

        case '+':

        case '*': return 3;

        case '.': return 2;

        case '|': return 1;

        default: return 0;

    }

}



// Converts an infix regex string to postfix

std::string RegexToDFA::regexToPostfix(const std::string& regex) {

    std::string postfix;

    std::stack<char> opStack;

    std::string processedRegex = addConcatOperator(regex);



    for (char c : processedRegex) {

        if (isalnum(c)) {

            postfix += c;

        } else if (c == '(') {

            opStack.push(c);

        } else if (c == ')') {

            while (!opStack.empty() && opStack.top() != '(') {

                postfix += opStack.top();

                opStack.pop();

            }

            if (!opStack.empty()) opStack.pop();

        } else {

            while (!opStack.empty() && opStack.top() != '(' && Priority(opStack.top()) >= Priority(c)) {

                postfix += opStack.top();

                opStack.pop();

            }

            opStack.push(c);

        }

    }



    while (!opStack.empty()) {

        postfix += opStack.top();

        opStack.pop();

    }

    return postfix;

}



// Public method to get the postfix expression

std::string RegexToDFA::getPostfix(const std::string& regex) {

    return regexToPostfix(regex);

}



// Public method to get the syntax tree (returns postfix representation)

std::string RegexToDFA::getSyntaxTree(const std::string& regex) {

    return getPostfix(regex);

}





// --- NFA to DFA Conversion ---



NFA RegexToDFA::postfixNFA(const std::string& postfix) {

    std::stack<NFA> nfaStack;

    for (char c : postfix) {

        if (isalnum(c)) {

            NFA nfa;

            State s0 = getNewState();

            State s1 = getNewState();

            nfa.states = {s0, s1};

            nfa.symbols.insert(c);

            nfa.startState = s0;

            nfa.finalStates = {s1};

            nfa.transitions[s0][c] = {s1};

            nfaStack.push(nfa);

        } else if (c == '.') {

            NFA nfa2 = nfaStack.top(); nfaStack.pop();

            NFA nfa1 = nfaStack.top(); nfaStack.pop();

            nfa1.states.insert(nfa2.states.begin(), nfa2.states.end());

            nfa1.symbols.insert(nfa2.symbols.begin(), nfa2.symbols.end());

            for (const auto& finalState : nfa1.finalStates) {

                nfa1.transitions[finalState]['\0'].insert(nfa2.startState);

            }

            nfa1.finalStates = nfa2.finalStates;

            nfa1.transitions.insert(nfa2.transitions.begin(), nfa2.transitions.end());

            nfaStack.push(nfa1);

        } else if (c == '|') {

            NFA nfa2 = nfaStack.top(); nfaStack.pop();

            NFA nfa1 = nfaStack.top(); nfaStack.pop();

            NFA newNfa;

            State s0 = getNewState();

            State s1 = getNewState();

            newNfa.states = {s0, s1};

            newNfa.states.insert(nfa1.states.begin(), nfa1.states.end());

            newNfa.states.insert(nfa2.states.begin(), nfa2.states.end());

            newNfa.symbols.insert(nfa1.symbols.begin(), nfa1.symbols.end());

            newNfa.symbols.insert(nfa2.symbols.begin(), nfa2.symbols.end());

            newNfa.startState = s0;

            newNfa.finalStates = {s1};

            newNfa.transitions[s0]['\0'] = {nfa1.startState, nfa2.startState};

            for (const auto& finalState : nfa1.finalStates) {

                newNfa.transitions[finalState]['\0'].insert(s1);

            }

            for (const auto& finalState : nfa2.finalStates) {

                newNfa.transitions[finalState]['\0'].insert(s1);

            }

            newNfa.transitions.insert(nfa1.transitions.begin(), nfa1.transitions.end());

            newNfa.transitions.insert(nfa2.transitions.begin(), nfa2.transitions.end());

            nfaStack.push(newNfa);

        } else if (c == '*') {

            NFA nfa1 = nfaStack.top(); nfaStack.pop();

            NFA newNfa;

            State s0 = getNewState();

            State s1 = getNewState();

            newNfa.states = {s0, s1};

            newNfa.states.insert(nfa1.states.begin(), nfa1.states.end());

            newNfa.symbols.insert(nfa1.symbols.begin(), nfa1.symbols.end());

            newNfa.startState = s0;

            newNfa.finalStates = {s1};

            newNfa.transitions[s0]['\0'] = {nfa1.startState, s1};

            for (const auto& finalState : nfa1.finalStates) {

                newNfa.transitions[finalState]['\0'].insert(nfa1.startState);

                newNfa.transitions[finalState]['\0'].insert(s1);

            }

            newNfa.transitions.insert(nfa1.transitions.begin(), nfa1.transitions.end());

            nfaStack.push(newNfa);

        }

    }

    return nfaStack.top();

}





std::set<State> RegexToDFA::lambdaClosure(const std::set<State>& states, const std::map<State, std::map<Symbol, std::set<State>>>& transitions) {

    std::set<State> closure = states;

    std::stack<State> stack;

    for (const auto& s : states) {

        stack.push(s);

    }

    while (!stack.empty()) {

        State current = stack.top();

        stack.pop();

        if (transitions.count(current) && transitions.at(current).count('\0')) {

            for (const auto& nextState : transitions.at(current).at('\0')) {

                if (closure.find(nextState) == closure.end()) {

                    closure.insert(nextState);

                    stack.push(nextState);

                }

            }

        }

    }

    return closure;

}



std::set<State> RegexToDFA::Move(const NFA& nfa, const std::set<State>& states, Symbol symbol) {

    std::set<State> result;

    for (const auto& state : states) {

        if (nfa.transitions.count(state) && nfa.transitions.at(state).count(symbol)) {

            const auto& nextStates = nfa.transitions.at(state).at(symbol);

            result.insert(nextStates.begin(), nextStates.end());

        }

    }

    return result;

}



std::string RegexToDFA::setToString(const std::set<State>& stateSet) {

    std::string str = "{";

    bool first = true;

    for (const auto& s : stateSet) {

        if (!first) str += ",";

        str += s;

        first = false;

    }

    str += "}";

    return str;

}



void RegexToDFA::NFATODFA(const NFA& nfa, std::set<State>& dfaStates, std::set<Symbol>& dfaAlphabet, TransitionFunction& dfaTransitions, State& dfaStartState, std::set<State>& dfaFinalStates) {

    dfaAlphabet = nfa.symbols;

    dfaAlphabet.erase('\0');



    std::map<std::set<State>, State> stateSetMap;

    std::stack<std::set<State>> unmarkedStates;



    std::set<State> startClosure = lambdaClosure({nfa.startState}, nfa.transitions);

    dfaStartState = setToString(startClosure);

    stateSetMap[startClosure] = dfaStartState;

    dfaStates.insert(dfaStartState);

    unmarkedStates.push(startClosure);

    

    for (const auto& s : startClosure) {

        if (nfa.finalStates.count(s)) {

            dfaFinalStates.insert(dfaStartState);

            break;

        }

    }



    while (!unmarkedStates.empty()) {

        std::set<State> currentSet = unmarkedStates.top();

        unmarkedStates.pop();

        State currentStateName = stateSetMap[currentSet];



        for (Symbol symbol : dfaAlphabet) {

            std::set<State> moveRes = Move(nfa, currentSet, symbol);

            std::set<State> nextSet = lambdaClosure(moveRes, nfa.transitions);



            if (nextSet.empty()) continue;



            if (stateSetMap.find(nextSet) == stateSetMap.end()) {

                State nextStateName = setToString(nextSet);

                stateSetMap[nextSet] = nextStateName;

                dfaStates.insert(nextStateName);

                unmarkedStates.push(nextSet);

                for (const auto& s : nextSet) {

                    if (nfa.finalStates.count(s)) {

                        dfaFinalStates.insert(nextStateName);

                        break;

                    }

                }

            }

            dfaTransitions[currentStateName][symbol] = stateSetMap[nextSet];

        }

    }

}



bool RegexToDFA::convertRegexToDFA(const std::string& regex, std::set<State>& states, std::set<Symbol>& alphabet, TransitionFunction& delta, State& initialState, std::set<State>& finalStates) {

    try {

        stateCounter = 0;

        std::string postfix = regexToPostfix(regex);

        NFA nfa = postfixNFA(postfix);

        NFATODFA(nfa, states, alphabet, delta, initialState, finalStates);

        return true;

    } catch (const std::exception& e) {

        std::cerr << "Error during Regex to DFA conversion: " << e.what() << std::endl;

        return false;

    }

}



void RegexToDFA::printDFA(const std::set<State>& states, const std::set<Symbol>& alphabet, const TransitionFunction& delta, const State& initialState, const std::set<State>& finalStates) {

    // This is a placeholder. The better print function is in DeterministicFiniteAutomaton.cpp

    std::cout << "DFA printing is handled by the DeterministicFiniteAutomaton class." << std::endl;

}







