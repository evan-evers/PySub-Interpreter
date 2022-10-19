//declaration of the expression evaluator class.

#ifndef EXPEVALUATOR_H
#define EXPEVALUATOR_H

#include <map>
#include <string>
#include "lexanalyzer.h"
#include "common.h"

class expEvaluator
{
private:
	typedef std::map<std::string, std::string> symbolTableType;
	symbolTableType symbolTable;
public:
	expEvaluator();
	tokenLineType infixToPostfix(const tokenLineType&);	//To convert an expression from infix to postfix
	int evaluatePostfix(const tokenLineType&);					//To evaluate the postfix expression
	void storeST(const std::string&, const std::string&);			//To store variables and values in the Symbol Table data structure
	std::string retrieveST(const std::string&);				//To find and retrieve a value from the Symbol Table data structure for a given variable
	void clearST();										//To clear or delete the Symbol Table data
	int operatorPrecedence(const std::string&);			//To return the precedence of an operator
};

#endif