//declaration of the interface class.

#ifndef INTERFACE_H
#define INTERFACE_H

#include "lexanalyzer.h"
#include "common.h"
#include <vector>
#include <string>

class Interface
{
private:
	programType programCode;	//programType is defined in "common.h"
public:
	Interface();
	void startInterface();
	void help() const;
	void help(std::string) const;
	void read(std::string, LexicalAnalyzer&);
	void show(std::string, LexicalAnalyzer&) const;
	void clear(bool, LexicalAnalyzer&);
};

#endif