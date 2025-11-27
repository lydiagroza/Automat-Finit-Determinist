// transformare se face asa: Regex -> AFN -> (prin minimizare) AFD

#include <stack>
#include<vector>
#include "DeterministicFiniteAutomaton.h"
class RegexToDFA
{
private:
    bool isOperator(char c){
        if (c == '.' || c == '|' || c == '*')
        {
            return true;
        }
        return false;
    }

    int Priority(char op){
        switch (op) {
        case '+': return 0;
        case '-': return 1;
        case '*': return 2;
        default: return 0;
        }

    }

public:

    std::string regexToPostfix(const std::string& regex) {
        std::stack<char> opStack;
        std::string postfix = ""; // Initialize empty

        for (char c : regex) {
            // 1. If operand, add to result
            if (std::isalnum(c)) {
                postfix += c;
            }
            // 2. If '(', push to stack
            else if (c == '(') {
                opStack.push(c);
            }
            // 3. If ')', pop until '(' is found
            else if (c == ')') {
                while (!opStack.empty() && opStack.top() != '(') {
                    postfix += opStack.top();
                    opStack.pop();
                }
                if (!opStack.empty()) {
                    opStack.pop(); // Pop the '('
                }
            }
            // 4. If operator, handle Priority
                while (!opStack.empty() &&
                       opStack.top() != '(' &&
                       Priority(opStack.top()) >= Priority(c)) {
                    postfix += opStack.top();
                    opStack.pop();
                       }
                opStack.push(c);
            }

        // Pop remaining operators
        while (!opStack.empty()) {
            postfix += opStack.top();
            opStack.pop();
        }

        return postfix;
    }
};



class PostFixToAFN
{
private:
    struct State
    {
        int id;
        std::multimap<char, State*> transitions;
        State(int id):id(id){}
    };

    struct NFA
    {
        State* start;
        State* end;
    };

    int StateCounter = 0;
    std::vector<State*> States;

    State* createState()
    {
        State*newState=new State(StateCounter++);
        States.push_back(newState);
        return newState;
    }



public:
    PostFixToAFN(const std::string& postfix) {
        std::stack<char> opStack;

        for (int index = 0; index < postfix.length(); index++)
        {
            if (std::isalpha(postfix[index]))
            {

            }
        }


    }
};



