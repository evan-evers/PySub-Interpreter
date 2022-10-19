//this file is used to store definitions shared by all or most classes, rather than redefining some typedefs and enums multiple times in different classes.
//it also has a to-do list for additional features.

#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>
#include <utility>

enum class categoryType
{
    KEYWORD,
    IDENTIFIER,
    STRING_LITERAL,
    NUMERIC_LITERAL,
    ASSIGNMENT_OP,
    ARITH_OP,
    LOGICAL_OP,
    RELATIONAL_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COMMA,
    COMMENT,
    INDENT,
    UNKNOWN
};

typedef std::vector<std::string> programType;

typedef std::vector<std::vector<std::pair<std::string, categoryType>>> tokenType;   //a vector of "tokenLineType"s
typedef std::vector<std::pair<std::string, categoryType>> tokenLineType;   //a vector of "pairType"s
typedef std::pair<std::string, categoryType> pairType;  //string is the token itself, categoryType is the token type

#endif

/*
ADDITIONAL FEATURES TO-DO:
    -Add expression evaluation for negative integers
    -Add a debug mode and a command to toggle it on/off
        -Should toggle showing infix/postfix tokens for in-console expression evaluation
        -Should toggle "existing data has been erased" and "tokens have been generated" messages
*/