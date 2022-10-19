//implementation of the lexical analyzer class.

#include "lexanalyzer.h"
#include "common.h"
#include <iostream>
#include <string>
#include <ctype.h>
#include <utility>
using namespace std;



//initializer
LexicalAnalyzer::LexicalAnalyzer()
{
	tokenInfo;
}

//turn a categoryType into a string for output purposes
string LexicalAnalyzer::getCategoryTypeString(const categoryType& catType)
{
	switch (catType)
	{
	case(categoryType::KEYWORD): return "KEYWORD";
	case(categoryType::IDENTIFIER): return "IDENTIFIER";
	case(categoryType::STRING_LITERAL): return "STRING_LITERAL";
	case(categoryType::NUMERIC_LITERAL): return "NUMERIC_LITERAL";
	case(categoryType::ASSIGNMENT_OP): return "ASSIGNMENT_OP";
	case(categoryType::ARITH_OP): return "ARITH_OP";
	case(categoryType::LOGICAL_OP): return "LOGICAL_OP";
	case(categoryType::RELATIONAL_OP): return "RELATIONAL_OP";
	case(categoryType::LEFT_PAREN): return "LEFT_PAREN";
	case(categoryType::RIGHT_PAREN): return "RIGHT_PAREN";
	case(categoryType::COLON): return "COLON";
	case(categoryType::COMMA): return "COMMA";
	case(categoryType::COMMENT): return "COMMENT";
	case(categoryType::INDENT): return "INDENT";
	case(categoryType::UNKNOWN): return "UNKNOWN";
	default: return "UNKNOWN";	//dunno if this is necessary but I like to be careful.
	}
}

//retrieve and return the token info
tokenType LexicalAnalyzer::getTokenInfo()
{
	return tokenInfo;
}

//set a line of tokenInfo; first variable is the value to set it to, second variable is the line that will be set
void LexicalAnalyzer::setTokenLineType(const tokenLineType& values, const int& i)
{
	tokenInfo[i].clear();	//clear the specified line of tokenInfo
	for (int j = 0; j < values.size(); j++)	//populate that line with the desired values
	{
		pairType temp(values[j].first, values[j].second);
		tokenInfo[i].push_back(temp);
	}
}

//show contents of tokenInfo
void LexicalAnalyzer::showTokenInfo()
{
	cout << endl;
	if (tokenInfo.size() > 0)	//if there's data in tokenInfo, show it
	{
		for (int i = 0; i < tokenInfo.size(); i++)
		{	//go through each line
			cout << "Line #" << i << ":" << endl;
			for (int j = 0; j < tokenInfo[i].size(); j++)
			{	//go through each token of each line
				cout << "Token[" << j << "]:   " << tokenInfo[i][j].first << " - " << getCategoryTypeString(tokenInfo[i][j].second) << endl;
			}
			cout << "------------------------------------------------------------------------" << endl;
		}

		cout << "\n\n";
	}
	else
	{	//if no data in tokenInfo, show a message
		cout << "No tokens in structure to show. To put tokens in the structure, use the 'read()' command.\n";
	}
}

//go through Interface.programCode and generate tokens. programType is defined in "common.h".
void LexicalAnalyzer::generateTokenInfo(const programType& code)
{
	/*
	Loop through every character, c, in a program line:

	if c is a digit, collect numeric literal as long as you read digits

	if c is a letter, collect an identifier as long as you read alphabetic characters (also check for keywords and Logical operators)

	if c is a single or double quote, collect a string literal as long as you read characters other than a single or double quote

	if c is a parenthesis, colon, or comma, classify it as LEFT_PAREN, RIGHT_PAREN, COLON, or COMMA as appropriate

	if c is #, collect comment to end of line

	if c is space character (i.e. space or tab), ignore it and read next character unless it is the beginning of the line, then store as INDENT

	if c is an operator, identify whether it is an Assignment, Relational, or Arithmetic operator

	Otherwise, classify as UNKNOWN
	*/

	//iterate through programCode (which is a vector of strings)
	for (int i = 0; i < code.size(); i++)
	{
		int j = 0;	//j is what character in "code" we're on
		tokenLineType temp_tlt = {};	//create tokenLineType that will store a row of tokens in tokenInfo

		while (j < code[i].length())	//while we're not at the end of the line
		{
			char c = code[i][j];

			if (isdigit(c))	//digit case (numeric literal)
			{
				//collect token
				string num_lit = "";
				do
				{
					num_lit += c;
					if (j >= code[i].length()) break;	//end if we hit the end of the line (avoids out-of-bounds access)
					j++;
					c = code[i][j];
				} while (isdigit(c));

				pairType temp_pt;			//make a new pair
				temp_pt.first = num_lit;	//assign values of the new pair
				temp_pt.second = categoryType::NUMERIC_LITERAL;
				temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt
			}
			else if (isalpha(c))	//letter case (identifiers, keywords and logical operators)
			{
				//collect token
				string letters = "";
				do
				{
					letters += c;
					if (j >= code[i].length()) break;	//end if we hit the end of the line (avoids out-of-bounds access)
					j++;
					c = code[i][j];
				} while (isalnum(c));	//i can't figure out if identifiers are allowed to have numbers in them or not, so i'm going to say they can have numbers past the first char, c++ style.

				pairType temp_pt;			//make a new pair
				temp_pt.first = letters;	//assign values of the new pair
				//assign categoryType according to what the token is
				if (letters == "if" || letters == "elif" || letters == "else" || letters == "while" || letters == "print" || letters == "int" || letters == "input")	//check for keyword
					temp_pt.second = categoryType::KEYWORD;
				else if (letters == "or" || letters == "and" || letters == "not")	//check for logical operator
					temp_pt.second = categoryType::LOGICAL_OP;
				else	//must be an identifier
					temp_pt.second = categoryType::IDENTIFIER;
				temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt
			}
			else if (c == '\'' || c == '\"')	//string case (string literal)
			{
				bool unknown = false;	//set to true if no end quote is found on the line

				//assign the end of the delimiter, so we can know when we properly get to the end of a string
				char quote_type;
				if (c == '\'')
					quote_type = '\'';
				else
					quote_type = '\"';

				//collect token
				string string_lit = "";
				do
				{
					string_lit += c;
					if (j >= code[i].length())
					{	//if we hit the end of the line without finding a matching quote, classify the token as an unknown
						unknown = true;
						break;
					}
					j++;
					c = code[i][j];
				} while (c != quote_type);

				//gotta do this to get the end quote
				string_lit += c;
				j++;

				pairType temp_pt;			//make a new pair
				temp_pt.first = string_lit;	//assign values of the new pair
				if (!unknown)	//if everything went fine & the token is not unknown, it's a string literal
					temp_pt.second = categoryType::STRING_LITERAL;
				else
					temp_pt.second = categoryType::UNKNOWN;
				temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt
			}
			else if (c == '(')	//left parenthesis case
			{
				pairType temp_pt;			//make a new pair
				temp_pt.first = c;	//assign values of the new pair
				temp_pt.second = categoryType::LEFT_PAREN;
				temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

				j++;	//increment index
			}
			else if (c == ')')	//right parenthesis case
			{
				pairType temp_pt;			//make a new pair
				temp_pt.first = c;	//assign values of the new pair
				temp_pt.second = categoryType::RIGHT_PAREN;
				temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

				j++;	//increment index
			}
			else if (c == ':')	//colon case
			{
				pairType temp_pt;			//make a new pair
				temp_pt.first = c;	//assign values of the new pair
				temp_pt.second = categoryType::COLON;
				temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

				j++;	//increment index
			}
			else if (c == ',')	//comma case
			{
				pairType temp_pt;			//make a new pair
				temp_pt.first = c;	//assign values of the new pair
				temp_pt.second = categoryType::COMMA;
				temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

				j++;	//increment index
			}
			else if (c == '#')	//comment case
			{
				//move forward one char in the string so we don't put the hashtag in the token
				j++;
				c = code[i][j];

				//collect token
				string comment = "";
				do
				{
					comment += c;
					if (j >= code[i].length()) break;	//end if we hit the end of the line (avoids out-of-bounds access)
					j++;
					c = code[i][j];
				} while (true);	//it's fine there's a break statement in there

				pairType temp_pt;			//make a new pair
				temp_pt.first = comment;	//assign values of the new pair
				temp_pt.second = categoryType::COMMENT;
				temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt
			}
			else if (c == ' ' || c == '\t')	//whitespace and indent case
			{
				if (j == 0)
				{	//if at beginning of line, collect indent
					//collect token
					string indent = "";
					do
					{
						indent += c;
						if (j >= code[i].length()) break;	//end if we hit the end of the line (avoids out-of-bounds access)
						j++;
						c = code[i][j];
					} while (c == ' ' || c == '\t');

					pairType temp_pt;			//make a new pair
					temp_pt.first = indent;	//assign values of the new pair
					temp_pt.second = categoryType::INDENT;
					temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt
				}
				else
				{	//if not at beginning of line, skip all whitespace
					//might as well make this a loop, so the algorythm isn't going through a dozen if else statements for every piece of consecutive whitespace
					do	//just increment, don't store
					{
						if (j >= code[i].length()) break;	//end if we hit the end of the line (avoids out-of-bounds access)
						j++;
						c = code[i][j];
					} while (c == ' ' || c == '\t');
				}
			}
			else if (c == '=' || c == '>' || c == '<' || c == '!')	//assignment and relational operator case
			{
				//set up some stuff for the two-character operators
				char c_next;
				bool next_exists;
				if ((j + 1) < code[i].length())
				{
					c_next = code[i][j + 1];
					next_exists = true;
				}
				else
					next_exists = false;

				switch (c)
				{	//handles each possible operator seperately
				case('='):
					if (next_exists && c_next == '=')
					{	//relational op
						pairType temp_pt;			//make a new pair
						temp_pt.first = "==";	//assign values of the new pair
						temp_pt.second = categoryType::RELATIONAL_OP;
						temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

						j += 2;	//increment index
					}
					else
					{	//assignment op
						pairType temp_pt;			//make a new pair
						temp_pt.first = c;	//assign values of the new pair
						temp_pt.second = categoryType::ASSIGNMENT_OP;
						temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

						j++;	//increment index
					}
					break;
				case('>'):
					if (next_exists && c_next == '=')
					{	//relational op
						pairType temp_pt;			//make a new pair
						temp_pt.first = ">=";	//assign values of the new pair
						temp_pt.second = categoryType::RELATIONAL_OP;
						temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

						j += 2;	//increment index
					}
					else
					{	//different relational op
						pairType temp_pt;			//make a new pair
						temp_pt.first = c;	//assign values of the new pair
						temp_pt.second = categoryType::RELATIONAL_OP;
						temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

						j++;	//increment index
					}
					break;
				case('<'):
					if (next_exists && c_next == '=')
					{	//relational op
						pairType temp_pt;			//make a new pair
						temp_pt.first = "<=";	//assign values of the new pair
						temp_pt.second = categoryType::RELATIONAL_OP;
						temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

						j += 2;	//increment index
					}
					else
					{	//different relational op
						pairType temp_pt;			//make a new pair
						temp_pt.first = c;	//assign values of the new pair
						temp_pt.second = categoryType::RELATIONAL_OP;
						temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

						j++;	//increment index
					}
					break;
				case('!'):
					if (next_exists && c_next == '=')
					{	//relational op
						pairType temp_pt;			//make a new pair
						temp_pt.first = ">=";	//assign values of the new pair
						temp_pt.second = categoryType::RELATIONAL_OP;
						temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

						j += 2;	//increment index
					}
					else
					{	//unknown
						pairType temp_pt;			//make a new pair
						temp_pt.first = c;	//assign values of the new pair
						temp_pt.second = categoryType::UNKNOWN;
						temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

						j++;	//increment index
					}
					break;
				}
			}
			else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')	//arithmetic operator case
			{
				pairType temp_pt;			//make a new pair
				temp_pt.first = c;	//assign values of the new pair
				temp_pt.second = categoryType::ARITH_OP;
				temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt

				j++;	//increment index
			}
			else	//unknown operator case (it's assumed this will be a 
			{
				//collect token
				string unknown = "";
				do
				{
					unknown += c;
					if (j >= code[i].length()) break;	//end if we hit the end of the line (avoids out-of-bounds access)
					j++;
					c = code[i][j];
				} while (isalnum(c));		//being careful; don't want to put a part of another token in the unknown token or go out of bounds

				pairType temp_pt;			//make a new pair
				temp_pt.first = unknown;	//assign values of the new pair
				temp_pt.second = categoryType::UNKNOWN;
				temp_tlt.push_back(temp_pt);	//put the new pair in temp_tlt
			}
		}
		//store the tokenLineType that was just put together in tokenInfo
		tokenInfo.push_back(temp_tlt);
	}

	cout << "\nTokens have been generated.\n";
}

//clear the tokenInfo data structure
void LexicalAnalyzer::clearTokenInfo()
{
	//eliminate each token entry, then eliminate the line entry.
	int size_col = tokenInfo.size();
	for (int i = size_col; i > 0; i--)
	{
		//dunno if it's necessary to free inner vectors but I like to be careful
		int size_row = tokenInfo[i - 1].size();
		for (int j = size_row; j > 0; j--)
		{
			tokenInfo[i - 1].pop_back();
		}
		tokenInfo.pop_back();
	}
}