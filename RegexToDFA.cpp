
#include <stack>
#include<vector>
#include"RegexToDFA.h"

int RegexToDFA::stateCounter = 0;

std::string RegexToDFA::addConcatOperator(const std::string& regex) {
    std::string result;
    for (size_t i = 0; i < regex.length(); i++) {
        result += regex[i];

        if (i + 1 < regex.length()) {
            char curr = regex[i];
            char next = regex[i + 1];

            // Add '.' for concatenation between:
            // letter/*/)/] followed by letter/(
            if ((isalnum(curr) || curr == '*' || curr == ')') &&
                (isalnum(next) || next == '(')) {
                result += '.';
                }
        }
    }
    return result;
}

int RegexToDFA::Priority(char op){
    switch (op) {
    case '.': return 1;
    case '|': return 2;
    case '*': return 3;
    default: return 0;
    }

}

std::string RegexToDFA:: regexToPostfix(const std::string& regex) {
        std::string postfix;
        std::stack<char> opStack;

        for (char c : regex) {
            if (isalnum(c)) {
                postfix += c;
            } else if (c == '(') {
                opStack.push(c);
            } else if (c == ')') {
                while (!opStack.empty() && opStack.top() != '(') {
                    postfix += opStack.top();
                    opStack.pop();
                }
                if (!opStack.empty()) opStack.pop(); // Remove '('
            } else { // operator
                while (!opStack.empty() && opStack.top() != '(' &&
                       Priority(opStack.top()) >= Priority(c)) {
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

NFA RegexToDFA::postfixNFA(const std::string& postfix){
    std::stack<NFA> nfaStack;

    for (char c : postfix)
    {
        if (isalnum(c))
        {
            NFA nfa;
            State start = getNewState();
            State final = getNewState();

            nfa.states.insert(start);
            nfa.states.insert(final);
            nfa.symbols.insert(c);
            nfa.transitions[start][c].insert(final);
            nfa.startState = start;
            nfa.finalStates.insert(final);

            nfaStack.push(nfa);  // <-- THIS WAS MISSING!

        } else if (c == '|')
        {
            NFA nfa2 = nfaStack.top(); nfaStack.pop();
            NFA nfa1 = nfaStack.top(); nfaStack.pop();

            NFA nfa;
            State newState = getNewState();
            State newFinal = getNewState();

            nfa.states.insert(newState);
            nfa.states.insert(newFinal);
            nfa.states.insert(nfa1.states.begin(), nfa1.states.end());
            nfa.states.insert(nfa2.states.begin(), nfa2.states.end());

            nfa.symbols.insert(nfa1.symbols.begin(), nfa1.symbols.end());
            nfa.symbols.insert(nfa2.symbols.begin(), nfa2.symbols.end());

            nfa.transitions[newState]['\0'].insert(nfa1.startState);
            nfa.transitions[newState]['\0'].insert(nfa2.startState);

            for (const auto& index : nfa1.finalStates)
            {
                nfa.transitions[index]['\0'].insert(newFinal);
            }
            for (const auto& index : nfa2.finalStates)
            {
                nfa.transitions[index]['\0'].insert(newFinal);
            }

            for (const auto& trans : nfa1.transitions)
            {
                for (const auto& symbol : trans.second)
                {
                    nfa.transitions[trans.first][symbol.first].insert(symbol.second.begin(), symbol.second.end());
                }
            }

            for (const auto& trans : nfa2.transitions)
            {
                for (const auto& symbol : trans.second)
                {
                    nfa.transitions[trans.first][symbol.first].insert(symbol.second.begin(), symbol.second.end());
                }
            }

            nfa.startState = newState;
            nfa.finalStates.insert(newFinal);

            nfaStack.push(nfa);

        } else if (c == '.')
        {
            NFA nfa2 = nfaStack.top(); nfaStack.pop();
            NFA nfa1 = nfaStack.top(); nfaStack.pop();

            NFA nfa;
            nfa.states.insert(nfa1.states.begin(), nfa1.states.end());
            nfa.states.insert(nfa2.states.begin(), nfa2.states.end());

            nfa.symbols.insert(nfa1.symbols.begin(), nfa1.symbols.end());
            nfa.symbols.insert(nfa2.symbols.begin(), nfa2.symbols.end());

            for (const auto& index : nfa1.finalStates)
            {
                nfa.transitions[index]['\0'].insert(nfa2.startState);
            }

            for (const auto& trans : nfa1.transitions)
            {
                for (const auto& symbol : trans.second)
                {
                    nfa.transitions[trans.first][symbol.first].insert(symbol.second.begin(), symbol.second.end());
                }
            }

            for (const auto& trans : nfa2.transitions)
            {
                for (const auto& symbol : trans.second)
                {
                    nfa.transitions[trans.first][symbol.first].insert(symbol.second.begin(), symbol.second.end());
                }
            }

            nfa.startState = nfa1.startState;
            nfa.finalStates = nfa2.finalStates;

            nfaStack.push(nfa);

        } else if (c == '*')
        {
            NFA nfa1 = nfaStack.top(); nfaStack.pop();

            NFA nfa;
            State newState = getNewState();
            State newFinal = getNewState();

            nfa.states.insert(newState);
            nfa.states.insert(newFinal);
            nfa.states.insert(nfa1.states.begin(), nfa1.states.end());

            nfa.symbols = nfa1.symbols;

            nfa.transitions[newState]['\0'].insert(nfa1.startState);
            nfa.transitions[newState]['\0'].insert(newFinal);

            for (const auto& index : nfa1.finalStates)
            {
                nfa.transitions[index]['\0'].insert(nfa1.startState);
                nfa.transitions[index]['\0'].insert(newFinal);
            }

            for (const auto& trans : nfa1.transitions) {
                for (const auto& symb : trans.second) {
                    nfa.transitions[trans.first][symb.first].insert(symb.second.begin(), symb.second.end());
                }
            }

            nfa.startState = newState;
            nfa.finalStates.insert(newFinal);

            nfaStack.push(nfa);
        }
    }

    return nfaStack.top();
}

std::set<State> RegexToDFA::lambdaClosure(const std::set<State>& states,
                                          const std::map<State, std::map<Symbol, std::set<State>>>& transitions){
    std::set<State> closure = states;
    std::stack<State> stack;

    for (const auto& s : states)
    {
        stack.push(s);
    }

    while (!stack.empty())
    {
        State current = stack.top();
        stack.pop();

        if (transitions.count(current) && transitions.at(current).count('\0'))
        {
            for (const auto& nextState : transitions.at(current).at('\0'))
            {
                if (closure.find(nextState) == closure.end())
                {
                    closure.insert(nextState);
                    stack.push(nextState);
                }
            }
        }
    }

    return closure;
}

std::string RegexToDFA::setToString(const std::set<State>& stateSet) {
    if (stateSet.empty()) return "{}";

    std::string result = "{";
    bool first = true;
    for (const auto& s : stateSet) {
        if (!first) result += ",";
        result += s;
        first = false;
    }
    result += "}";
    return result;
}

void RegexToDFA::NFATODFA(const NFA& nfa,
                          std::set<State>& dfaStates,
                          std::set<Symbol>& dfaSymbols,
                          TransitionFunction& dfaTransitions,
                          State& dfaStartState,
                          std::set<State>& dfaFinalStates)
{
    dfaSymbols = nfa.symbols;

    std::map<std::set<State>, State> setToStateName;
    std::stack<std::set<State>> stack;

    std::set<State> startSet = lambdaClosure({nfa.startState}, nfa.transitions);
    dfaStartState = setToString(startSet);
    setToStateName[startSet] = dfaStartState;
    dfaStates.insert(dfaStartState);
    stack.push(startSet);

    for (const auto& s : startSet) {
        if (nfa.finalStates.count(s)) {
            dfaFinalStates.insert(dfaStartState);
            break;
        }
    }

    while (!stack.empty()) {
        std::set<State> currentSet = stack.top();
        stack.pop();

        State currentDFAState = setToStateName[currentSet];

        for (char symbol : dfaSymbols) {
            std::set<State> nextSet;

            // Find all states reachable by symbol from current set
            for (const auto& s : currentSet) {
                if (nfa.transitions.count(s) && nfa.transitions.at(s).count(symbol)) {
                    for (const auto& next : nfa.transitions.at(s).at(symbol)) {
                        nextSet.insert(next);
                    }
                }
            }

            // Apply epsilon closure
            nextSet = lambdaClosure(nextSet, nfa.transitions);

            if (!nextSet.empty()) {
                State nextDFAState;

                if (setToStateName.find(nextSet) == setToStateName.end()) {
                    nextDFAState = setToString(nextSet);
                    setToStateName[nextSet] = nextDFAState;
                    dfaStates.insert(nextDFAState);
                    stack.push(nextSet);

                    // Check if this is an accepting state
                    for (const auto& s : nextSet) {
                        if (nfa.finalStates.count(s)) {
                            dfaFinalStates.insert(nextDFAState);
                            break;
                        }
                    }
                } else {
                    nextDFAState = setToStateName[nextSet];
                }

                dfaTransitions[currentDFAState][symbol] = nextDFAState;
            }
        }
    }
}

bool RegexToDFA::convertRegexToDFA(const std::string& regex,
                                   std::set<State>& states,
                                   std::set<Symbol>& alphabet,
                                   TransitionFunction& delta,
                                   State& initialState,
                                   std::set<State>& finalStates) {
    try {
        stateCounter = 0; // Reset counter

        // Step 1: Add explicit concatenation
        std::string withConcat = addConcatOperator(regex);

        // Step 2: Convert to postfix
        std::string postfix = regexToPostfix(withConcat);

        std::cout << "Postfix expression: " << postfix << std::endl;

        // Step 3: Build NFA
        NFA nfa = postfixNFA(postfix);

        // Step 4: Convert NFA to DFA
        NFATODFA(nfa, states, alphabet, delta, initialState, finalStates);

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error converting regex to DFA: " << e.what() << std::endl;
        return false;
    }

}

void RegexToDFA::printDFA(const std::set<State>& states,
                          const std::set<Symbol>& alphabet,
                          const TransitionFunction& delta,
                          const State& initialState,
                          const std::set<State>& finalStates) {

    std::cout << "\n╔════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║              DFA TRANSITION TABLE                      ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════╝\n" << std::endl;

    // Calculate column width based on longest state name
    size_t maxStateLen = 8;
    for (const auto& state : states) {
        if (state.length() > maxStateLen) {
            maxStateLen = state.length();
        }
    }

    // Print header with alphabet symbols
    std::cout << "State";
    for (size_t i = 5; i < maxStateLen; i++) {
        std::cout << " ";
    }
    std::cout << "  ";

    for (const auto& symbol : alphabet) {
        std::cout << "| " << symbol << "       ";
    }
    std::cout << std::endl;

    // Print separator line
    for (size_t i = 0; i < maxStateLen + 2; i++) {
        std::cout << "-";
    }
    for (size_t i = 0; i < alphabet.size(); i++) {
        std::cout << "|--------";
    }
    std::cout << std::endl;

    // Print each state and its transitions
    for (const auto& state : states) {
        // Mark initial state with → and final states with *
        bool isInitial = (state == initialState);
        bool isFinal = (finalStates.find(state) != finalStates.end());

        std::string marker = "  ";
        if (isInitial && isFinal) {
            marker = "→*";
        } else if (isInitial) {
            marker = "→ ";
        } else if (isFinal) {
            marker = " *";
        }

        std::cout << marker;

        // Print state name
        std::string stateName = state;
        if (stateName.length() > 15) {
            stateName = stateName.substr(0, 12) + "...";
        }
        std::cout << stateName;

        // Pad to align columns
        for (size_t i = stateName.length(); i < maxStateLen; i++) {
            std::cout << " ";
        }

        // Print transitions for each symbol
        for (const auto& symbol : alphabet) {
            std::cout << "| ";

            if (delta.count(state) && delta.at(state).count(symbol)) {
                std::string nextState = delta.at(state).at(symbol);
                if (nextState.length() > 6) {
                    nextState = nextState.substr(0, 5) + "..";
                }
                std::cout << nextState;

                // Pad to align
                for (size_t i = nextState.length(); i < 6; i++) {
                    std::cout << " ";
                }
            } else {
                std::cout << "---   ";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "\n" << std::endl;

    // Print legend
    std::cout << "Legend:" << std::endl;
    std::cout << "  → : Initial state" << std::endl;
    std::cout << "  * : Accept/Final state" << std::endl;
    std::cout << "  →*: Initial and Accept state" << std::endl;
    std::cout << "  ---: No transition defined" << std::endl;

    // Print statistics
    std::cout << "\n════════════════════════════════════════" << std::endl;
    std::cout << "DFA Statistics:" << std::endl;
    std::cout << "════════════════════════════════════════" << std::endl;
    std::cout << "  Total states: " << states.size() << std::endl;
    std::cout << "  Alphabet size: " << alphabet.size() << std::endl;
    std::cout << "  Alphabet: { ";
    for (const auto& sym : alphabet) {
        std::cout << sym << " ";
    }
    std::cout << "}" << std::endl;
    std::cout << "  Initial state: " << initialState << std::endl;
    std::cout << "  Accept states (" << finalStates.size() << "): ";

    bool first = true;
    for (const auto& fs : finalStates) {
        if (!first) std::cout << ", ";
        std::cout << fs;
        first = false;
    }
    std::cout << std::endl;
    std::cout << "════════════════════════════════════════\n" << std::endl;
}





