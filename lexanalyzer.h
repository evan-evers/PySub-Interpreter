//declaration of the lexical analyzer class.

#ifndef LEXANALYZER_H
#define LEXANALYZER_H

#include <vector>
#include <string>
#include <utility>
#include <list>
#include "common.h"

class LexicalAnalyzer
{
private:
    tokenType tokenInfo;
public:
    LexicalAnalyzer();
    std::string getCategoryTypeString(const categoryType&); //turn a categoryType into a string for output purposes
    tokenType getTokenInfo();   //retrieve and return the token info
    void setTokenLineType(const tokenLineType&, const int& i);   //set a line of tokenInfo; first variable is the value to set it to, second variable is the line that will be set
    void showTokenInfo();   //show contents of tokenInfo
    void generateTokenInfo(const programType&);  //go through Interface.programCode and generate tokens. programType is defined in "common.h".
    void clearTokenInfo();  //clear the tokenInfo data structure
};

#endif