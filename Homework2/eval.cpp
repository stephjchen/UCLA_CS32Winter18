//
//  eval.cpp
//  hw2

//

#include "Map.h"
#include <string>
#include <stack>
#include <cctype>

using namespace std;

int performOperation(int v1, int v2, char operation);
int opWeight (char op);
bool precedence(char op1, char op2);
bool isOperator (char op);
bool checkparenthesis (string infix);
string ridSpace (string& infix);
bool evalinfix (string infix);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    if (!evalinfix(infix))
        return 1;
    
    postfix = ""; //set postfix to empty string
    stack<char> opStack;     // declare a stack of char
    
    bool inMap = true;
    
    for (int i=0; i < infix.size(); i++)
    {
        if (islower(infix[i]))
        {
            postfix += infix[i];
            
            if(!values.contains(infix[i]))
                inMap = false;
        }
        
        else if (infix[i] == '(')
            opStack.push(infix[i]);
        
        else if (infix[i] == ')')
        {
            //pop stack until matching '('
            while(opStack.top()!= '(')
            {
                postfix += opStack.top();
                opStack.pop();
            }
            
            //remove the '('
            opStack.pop();
        }
        
        else if (isOperator(infix[i]))
        {
            while(!opStack.empty() && opStack.top()!='(' && precedence(infix[i], opStack.top()))
            {       postfix += opStack.top();
                    opStack.pop();
            }
            opStack.push(infix[i]);
        }
        
        else if (isspace(infix[i]))
            continue;
            
        else
            return 1; //invalid syntax
        
    }
    
    while (!opStack.empty())
    {
        postfix += opStack.top();
        opStack.pop();
    }
    //opStack should be empty here
    //finish coverting infix to postfix
    
    if (!inMap)
        return 2;
    
    
    stack<int> numStack; //initilaize an empty stack for the operand
    int v, v1, v2 = 0;
    int temp = 0;
    
    for (int i=0; i<postfix.size(); i++)
    {
        if (islower(postfix[i]))
        {
            values.get(postfix[i],v);
            numStack.push(v);
        }
        else //the character is an operator
        {
            v2 = numStack.top();
            numStack.pop();
            v1 = numStack.top();
            numStack.pop();
            
            if (postfix[i] == '/' && v2 == 0)
                return 3;
            
            temp = performOperation(v1, v2, postfix[i]);
            numStack.push(temp);
           
        }
    }
    
    result = numStack.top();
    return 0;
    
}

int performOperation(int v1, int v2, char operation)
{
    switch (operation)
    {
        case '+':
            return v1 + v2;
            break;
            
        case '-':
            return v1 - v2;
            break;
            
        case '*':
            return v1 * v2;
            break;
            
        case '/':
            return v1/v2;
            break;
            
        default: //should not get here
            return -4567;
            break;
    }
}

int opWeight (char op)
{
    int weight = -1;
    
    switch (op)
    {
        case '+':
        case '-':
            weight = 1;
            break;
            
        case '/':
        case '*':
            weight = 2;
            break;
    }
    
    return weight;
}

bool precedence(char op1, char op2)
{
    int weight1 = opWeight(op1);
    int weight2 = opWeight(op2);
    
    if (weight1 <= weight2)
        return true;
    
    return false;
    
}

bool isOperator (char op)
{
    if (op == '/' || op == '*' || op == '+' || op == '-')
        return true;
    
    return false;
}

bool checkparenthesis (string infix)
{
    stack<char> pStack;
    
    for (int i=0; i<infix.size(); i++)
    {
        if (infix[i] == '(')
            pStack.push(infix[i]);
        
        else if (infix[i] == ')') //ensures each parenthesis are in pairs
        {
            if (pStack.empty())
                return false;
            pStack.pop();
        }
    }
    
    if (!pStack.empty())
        return false;
    
    return true;
}

string ridSpace (string& infix)
{
    int size = infix.size();
    
    for (int i=0; i < size; i++)
        if (isspace(infix[i]))
        {
            infix.erase(i,1);
            size--;
            i--;
        }
    
    return infix;
}

bool evalinfix (string infix) //creates a copy of infix
{
    ridSpace(infix);
    
    //check if infix has anything other than parenthesis, operators, or operands
    for (int i=0; i < infix.size(); i++)     {
        
        if (islower(infix[i]) || infix[i] == '(' || infix[i] == ')' || isOperator(infix[i])) //continue if character is a lower case letter, an operator, parenthesis
            continue;
        
        else //invalid string if infix contains anything else (not even space)
            return false;
    }
    
    if (!checkparenthesis(infix)) //check if infix has valid parenthesis
        return false;
    
    if (isOperator(infix[0]) || isOperator(infix[infix.size()-1]) || infix == "") //operator is first or last in infix string or infix is empty string
        return false;
    
    char curr = '\0';
    char prev = '\0';
    
    //check if any consecutive operators or numbers
    for (int i=1; i < infix.size(); i++)
    {
        curr = infix[i]; //start examining from 1st element in string
        prev = infix[i-1];
        
        if (isOperator(curr)) //if the current is an operator
        {
            if (isOperator(prev) || prev == '(')
                return false;
        }
        
        else if (islower(curr)) //if the current is a number (char)
        {
            if (islower(prev) || prev == ')')
                return false;
        }
        
        else if (curr == '(')
        {
            if (islower(prev) || prev == ')')
                return false;
        }
        
        else if (curr == ')')
        {
            if (isOperator(prev) || prev == '(')
                return false;
        }
        
    }
    
    return true;
}
