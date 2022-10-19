//implementation of the expression evaluator class.

#include "expevaluator.h"
#include "lexanalyzer.h"
#include "common.h"
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <utility>
using namespace std;

expEvaluator::expEvaluator()	//default constructor
{
	symbolTable;
}

//To convert an expression from infix to postfix
tokenLineType expEvaluator::infixToPostfix(const tokenLineType& infix_vect)
{
	tokenLineType postfix_vect;	//will be returned at the end of the function
	stack<pairType> stack_pair;	//for storing operators and parentheses

	//go through the infix vector and convert it to a postfix expression
	for (int i = 0; i < infix_vect.size(); i++)
	{
		switch (infix_vect[i].second)
		{	//process element according to what it is
		case(categoryType::LEFT_PAREN):
			stack_pair.push(infix_vect[i]);	//add left parenthesis to the stack
			break;

		case(categoryType::RIGHT_PAREN):
			if (stack_pair.top().second == categoryType::LEFT_PAREN)	//if there's just a parenthesis on top of the stack, pop it
				stack_pair.pop();
			else	//operator on top with a left paren below it
			{
				while (stack_pair.top().second != categoryType::LEFT_PAREN)
				{	//put the operator in the postfix vect and pop
					postfix_vect.push_back(stack_pair.top());
					stack_pair.pop();
				}
				stack_pair.pop();	//pop left paren
			}
			break;

		case(categoryType::ARITH_OP):
		case(categoryType::LOGICAL_OP):
		case(categoryType::RELATIONAL_OP):
			if (stack_pair.size() > 0)
			{	//if there's something in the stack right now
				if (stack_pair.top().second == categoryType::LEFT_PAREN)	//if there's a  (  on top of the stack
					stack_pair.push(infix_vect[i]);	//add operator to the stack
				else	//if there's an operator on top of the stack
				{
					if (operatorPrecedence(stack_pair.top().first) < operatorPrecedence(infix_vect[i].first))
						stack_pair.push(infix_vect[i]);	//if precedence of current operator is greater than what's on the stack, put the operator on the stack
					else
					{	//curent operator is of equal or lower precedence
						//repeatedly put the operator at the top of the stack in the postfix vect and pop stack_pair, until an operator of lower precedence is found
						while (stack_pair.size() > 0 && operatorPrecedence(stack_pair.top().first) >= operatorPrecedence(infix_vect[i].first))
						{
							postfix_vect.push_back(stack_pair.top());
							stack_pair.pop();
						}
						stack_pair.push(infix_vect[i]);	//put current operator in stack
					}
				}
			}
			else
			{	//if there's nothing on the stack, put the operator on the stack
				stack_pair.push(infix_vect[i]);
			}
			break;

		case(categoryType::NUMERIC_LITERAL):	//this needs to work with variable names eventually
			postfix_vect.push_back(infix_vect[i]);	//add literal to the postfix vector
			break;

		case(categoryType::INDENT):	//in case the user entered a space before their expression when using the in-console evaluator
			//do nothing!
			break;

		default:	//if we come across an element of an improper type
			throw string("ERROR: Unexpected element in expression.");
			break;
		}
	}

	//empty any operators left in the stack
	while (stack_pair.size() > 0)
	{
		postfix_vect.push_back(stack_pair.top());
		stack_pair.pop();
	}

	return postfix_vect;
}

//To evaluate the postfix expression
int expEvaluator::evaluatePostfix(const tokenLineType& postfix_vect)
{
	stack<int> stack_operands;	//stack for storing operands

	//go through the postfix expression and evaluate it
	for (int i = 0; i < postfix_vect.size(); i++)
	{
		if (postfix_vect[i].second == categoryType::NUMERIC_LITERAL)
		{	//if the element is a number, convert it from string to integer and put it on the stack
			stack_operands.push(stoi(postfix_vect[i].first));
		}
		else
		{	//if the element is an operator, take the top 2 numbers on the stack, perform the operation on them, and sotre the result in the stack
			char c0 = postfix_vect[i].first[0], c1 = 0;	//vars that store the first and second elements of the string in postfix_vect[i].first
			if (postfix_vect[i].first.size() > 1)	c1 = postfix_vect[i].first[1];	//if there's more than one character in the string, give c1 the value of the second character
			if (stack_operands.empty())		//exception handling: throw an error if there's nothing in the stack
				throw string("ERROR: Missing operands.");
			int last_top = stack_operands.top(), result;	//vars to store right operand and result of an expression
			stack_operands.pop();	//pop to gain access to the next element

			switch (c0)
			{	//evaluate subexpression according to operator
			case('*'):
				if (stack_operands.empty())		//exception handling: throw an error if there's nothing in the stack
					throw string("ERROR: Missing operands.");

				result = stack_operands.top() * last_top;
				stack_operands.pop();	//eliminate top of stack and replace with result
				stack_operands.push(result);
				break;

			case('/'):
				if (stack_operands.empty())		//exception handling: throw an error if there's nothing in the stack
					throw string("ERROR: Missing operands.");
				if (last_top == 0)	//exception handling: throw an error if there's division by zero.
					throw string("ERROR: Division by zero.");

				result = stack_operands.top() / last_top;
				stack_operands.pop();	//eliminate top of stack and replace with result
				stack_operands.push(result);
				break;

			case('%'):
				if (stack_operands.empty())		//exception handling: throw an error if there's nothing in the stack
					throw string("ERROR: Missing operands.");

				result = stack_operands.top() % last_top;
				stack_operands.pop();	//eliminate top of stack and replace with result
				stack_operands.push(result);
				break;

			case('+'):
				if (stack_operands.empty())		//exception handling: throw an error if there's nothing in the stack
					throw string("ERROR: Missing operands.");

				result = stack_operands.top() + last_top;
				stack_operands.pop();	//eliminate top of stack and replace with result
				stack_operands.push(result);
				break;

			case('-'):
				if (stack_operands.empty())		//exception handling: throw an error if there's nothing in the stack
					throw string("ERROR: Missing operands.");

				result = stack_operands.top() - last_top;
				stack_operands.pop();	//eliminate top of stack and replace with result
				stack_operands.push(result);
				break;

			case('<'):
				if (stack_operands.empty())		//exception handling: throw an error if there's nothing in the stack
					throw string("ERROR: Missing operands.");

				if (c1 != 0)	//<=
				{
					if (stack_operands.top() <= last_top) result = 1; else result = 0;
				}
				else	//<
				{
					if (stack_operands.top() < last_top) result = 1; else result = 0;
				}
				stack_operands.pop();	//eliminate top of stack and replace with result
				stack_operands.push(result);
				break;

			case('>'):
				if (stack_operands.empty())		//exception handling: throw an error if there's nothing in the stack
					throw string("ERROR: Missing operands.");

				if (c1 != 0)	//>=
				{
					if (stack_operands.top() >= last_top) result = 1; else result = 0;
				}
				else	//>
				{
					if (stack_operands.top() > last_top) result = 1; else result = 0;
				}
				stack_operands.pop();	//eliminate top of stack and replace with result
				stack_operands.push(result);
				break;

			case('='):	//this can only be ==; there's no assignment operators mid-expression in PySub.
				if (stack_operands.empty())		//exception handling: throw an error if there's nothing in the stack
					throw string("ERROR: Missing operands.");

				if (stack_operands.top() == last_top) result = 1; else result = 0;
				stack_operands.pop();	//eliminate top of stack and replace with result
				stack_operands.push(result);
				break;

			case('n'):	//not
				if (last_top == 0) result = 1; else result = 0;	//invert truth value (1 = true, 0 = false)
				stack_operands.push(result);	//eliminate top of stack and replace with result
				break;

			case('a'):	//and
				if (stack_operands.empty())		//exception handling: throw an error if there's nothing in the stack
					throw string("ERROR: Missing operands.");

				if (stack_operands.top() == 1 && last_top == 1) result = 1; else result = 0;
				stack_operands.pop();	//eliminate top of stack and replace with result
				stack_operands.push(result);
				break;

			case('o'):	//or
				if (stack_operands.empty())		//exception handling: throw an error if there's nothing in the stack
					throw string("ERROR: Missing operands.");

				if (stack_operands.size() < 2)	//exception handling; in case of not enough operands.
					throw string("ERROR: Not enough operands for operator 'or'.");

				if (stack_operands.top() == 1 || last_top == 1) result = 1; else result = 0;
				stack_operands.pop();	//eliminate top of stack and replace with result
				stack_operands.push(result);
				break;
			}
		}
	}

	return stack_operands.top();	//should only be one element left in the stack, which should be the solution to the problem.
}

//To store variables and values in the Symbol Table data structure
void expEvaluator::storeST(const std::string& key, const std::string& val)
{
	symbolTable.insert(pair<string, string>(key, val));
}

//To find and retrieve a value from the Symbol Table data structure for a given variable
std::string expEvaluator::retrieveST(const std::string& key)
{
	if (symbolTable.find(key) != symbolTable.end())	//if value is found in symbolTable
		return symbolTable[key];
	else
		return "Error: key not found";	//error checking
}

//To clear or delete the Symbol Table data
void expEvaluator::clearST()
{
	symbolTable.clear();
}

//To return the precedence of an operator
int expEvaluator::operatorPrecedence(const std::string& s)
{
	if (s == "*" || s == "/" || s == "%")
	{
		return 5;
	}
	else if (s == "+" || s == "-")	//binary plus & minus operators
	{
		return 4;
	}
	else if (s == "<" || s == "<=" || s == ">" || s == ">=" || s == "!=" || s == "==")
	{
		return 3;
	}
	else if (s == "not")
	{
		return 2;
	}
	else if (s == "and")
	{
		return 1;
	}
	else if (s == "or")
	{
		return 0;
	}
	else					//error case
	{
		return -1;
	}
}