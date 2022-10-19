//implementation of the interface class.

#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"
#include "common.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;



//initializer
Interface::Interface()
{
	programCode;	//the raw python code.
}

//starts interface and contains the main input loop
void Interface::startInterface()
{
	string str;
	LexicalAnalyzer lex_analysis;
	expEvaluator exp_evaluator;

	//These lines show show up once at the beginning of the program
	cout << "PySUB Interpreter 1.0 on Windows (September 2022)" << endl;
	cout << "Enter program lines or read(<filename>.py) at command line interface" << endl;
	cout << "Type 'help' for more information or 'quit' to exit" << endl;

	//vars for string parsing
	int i;
	bool read_param, is_command;

	//Input loop
	while (true)
	{
		//set/reset variables
		i = 0;
		read_param = false;
		is_command = true;

		//get input
		cout << "\n>>> ";
		getline(cin, str);

		//check for digit in str; if there's a digit in str, user must have typed an expression
		while (i < str.size())
		{
			if (isdigit(str[i]))
			{
				is_command = false;
				break;
			}
			i++;
		}
		i = 0;	//reset i

		if (is_command)	//evaluate as command
		{
			//remove any whitespace from the string
			for (int j = 0; j < str.length(); j++)
			{
				if (str[j] == ' ' || str[j] == '\t')
					str.erase(j, 1);
			}

			//parse "str" to get the command and parameter.
			string command, parameter;
			while (str[i])
			{
				if (read_param)
				{
					if (str[i] != ')')
						parameter += str[i];
					else
						break;	//quit the loop when you hit a right parenthesis
				}
				else
				{
					if (str[i] != '(')
						command += str[i];
					else
						read_param = true;	//stop getting the command and start getting the parameter
				}

				i++;
			}

			//this would be so much nicer if switch statements accepted strings. oh well
			//in theory i could make seperate error messages for improper commands and improper parameters with this. would be cool
			if (command == "quit" && parameter == "")	//quit command; no param
				break;
			else if (command == "help" && parameter == "")	//help command w/ no parameter
				help();
			else if (command == "help" && parameter != "")	//help command with a parameter
				help(parameter);
			else if (command == "read" && parameter != "")	//read command with a parameter
				read(parameter, lex_analysis);
			else if (command == "show")	//show command with or without a parameter
				show(parameter, lex_analysis);
			else if (command == "clear" && parameter == "")	//clear command; no param. show message
				clear(true, lex_analysis);
			else
				cout << "\nCommand not recognized. Type 'help' for a list of commands.\n";
		}
		else //evaluate as an expression
		{	//this is a truncated version of the read() command
			if (programCode.size() != 0)
			{	//erase data if there's any in the interface right now
				clear(false, lex_analysis);
				cout << "\nExisting data has been erased.\n";
			}

			try
			{
				//this next bit should probably be streamlined; there's a lot of weird class member access going on here.
				programCode.push_back(str);	//put the expression to be evaluated in programCode
				lex_analysis.generateTokenInfo(programCode);	//generate tokens
				//cout << "Infix vector tokens: \n";	show("tokens", lex_analysis);	//here for debug purposes
				lex_analysis.setTokenLineType(exp_evaluator.infixToPostfix(lex_analysis.getTokenInfo()[0]), 0);	//convert tokens to a postfix expression
				//cout << "Postfix vector tokens: \n";	show("tokens", lex_analysis);	//here for debug purposes
				cout << "\nYour expression is equal to: " << exp_evaluator.evaluatePostfix(lex_analysis.getTokenInfo()[0]) << "\n";	//evaluate and output expression
			}
			catch (string error_message)
			{
				cout << endl << error_message << endl;
			}
		}
	}
}

void Interface::help() const
{	//this and help(string) should remain separate commands, as their code and function are quite different.
	string _command;

	//help start message
	cout << "\nWelcome to the help utility!" << endl;
	cout << "* To exit and return to the interpreter, type 'exit_help'" << endl;
	cout << "* To get a list of commands, type 'commands'" << endl;
	cout << "* To get a description of any command, just type the command at the help> prompt" << endl;

	//Input loop
	while (true)
	{
		//get input
		cout << "\nhelp> ";
		getline(cin, _command);

		if (_command == "exit_help" || _command == "exit_help()")
		{
			cout << "\nYou have exited the help utility.\n";
			break;
		}
		else if (_command == "commands" || _command == "commands()")
		{
			cout << "\nBelow is a list of commands. Enter any command at the prompt to get more help.\n";
			cout << "NOTE: All commands can also be entered as functions.\n\n";
			cout << "* clear ~or~ clear()\n" << "* help ~or~ help()\n" << "* quit ~or~ quit()\n" << "* read ~or~ read()\n" << "* show ~or~ show()\n";
		}
		else if (_command == "clear" || _command == "clear()")
		{
			cout << "\n'clear' clears out or deletes any lines stored in the program data structure.\n";
		}
		else if (_command == "help" || _command == "help()")
		{
			cout << "\n'help' enters the help utility, which you are currently using.\n";
			cout << "The help utility shows what commands are available and explains their use.\n";
			cout << "Typing help(<command>) outside of the help utility will give a description of what <command> does.\n";
		}
		else if (_command == "quit" || _command == "quit()")
		{
			cout << "\n'quit' exits the interpreter interface.\n";
		}
		else if (_command == "read" || _command == "read()")
		{
			cout << "\n'read(<filename.py>)' clears any lines in the program data structure.\n";
			cout << "It then reads the contents of <filename.py> into the program data structure.\n";
		}
		else if (_command == "show" || _command == "show()")
		{
			cout << "\n'show' shows the lines of the program that are stored in the programCode data structure.\n";
			cout << "'show(tokens)' shows the tokens that are stored in the tokenInfo data structure.\n";
		}
		else
			cout << "\nCommand not recognized.\n";
	}
}

void Interface::help(string _command) const
{	//this and help() should remain separate commands, as their code and function are quite different.
	if (_command == "clear")
	{
		cout << "\n'clear' clears out or deletes any lines stored in the program data structure.\n";
	}
	else if (_command == "help")
	{
		cout << "\n'help' enters the help utility.\n";
		cout << "The help utility shows what commands are available and explains their use.\n";
		cout << "Typing help(<command>) outside of the help utility will give a description of what <command> does.\n";
		cout << "It's kind of weird that you would check what this does this way instead of just entering the help utiliy.\n";
		cout << "But you do you, I guess.\n";
	}
	else if (_command == "quit")
	{
		cout << "\n'quit' exits the interpreter interface.\n";
	}
	else if (_command == "read")
	{
		cout << "\n'read(<filename.py>)' clears any lines in the program data structure.\n";
		cout << "It then reads the contents of <filename.py> into the program data structure.\n";
	}
	else if (_command == "show")
	{
		cout << "\n'show' shows the lines of the program that are stored in the programCode data structure.\n";
		cout << "'show(tokens)' shows the tokens that are stored in the tokenInfo data structure.\n";
	}
	else
		cout << "\nCommand not recognized. Type 'help' without a parameter for a list of commands.\n";
}

void Interface::read(string filename, LexicalAnalyzer& lexA)
{
	//make file object and open it
	ifstream ifile;
	ifile.open(filename);

	if (!ifile)
	{
		cout << "\nError: file could not be opened.\n";
		return;
	}
	else
	{
		//message telling user that existing data will be erased (NOTE: data has not actually been erased yet. message goes here for convenience)
		if (programCode.size() != 0)
		{
			//clear programCode only if the file opens properly
			clear(false, lexA);
			cout << "\nExisting data has been erased.\n";
		}

		int i = 0;	//incrementer to access the right element of programCode
		while (true)
		{
			if (!ifile)
				break;

			string temp;
			getline(ifile, temp);
			//temp = "[" + to_string(i) + "] " + temp;	//properly format temp
			programCode.push_back(temp);

			i++;
		}

		//confirmation msg
		cout << "\nFile is now stored in the data structure.\n";

		//generate tokens
		lexA.generateTokenInfo(programCode);
	}

	//close file
	ifile.close();
}

void Interface::show(string param, LexicalAnalyzer& lexA) const
{
	if (param == "")
	{	//if no parameter is given, show the raw code
		if (programCode.size() != 0)
		{
			cout << endl;
			for (int i = 0; i < programCode.size(); i++)	//for each string in the vector
				cout << "[" << to_string(i) << "] " << programCode[i] << endl;
		}
		else
			cout << "\nNo data in structure to show. To put data in the structure, use the 'read()' command.\n";
	}
	else if (param == "tokens")
	{	//if parameter "tokens" is given, show the tokens
		lexA.showTokenInfo();
	}
	else
	{	//inproper input; error message
		cout << "\nCommand not recognized. Type 'help' without a parameter for a list of commands.\n";
	}
}

void Interface::clear(bool show_msg, LexicalAnalyzer& lexA)
{
	if (programCode.size() != 0)
	{
		int check = programCode.size();
		for (int i = 0; i < check; i++)	//for each string in the vector
			programCode.pop_back();

		if (show_msg)	//show message if the bool is true
			cout << "\nData structure has been cleared.\n";

		lexA.clearTokenInfo();
	}
	else
		if (show_msg)
			cout << "\nNo data in structure to clear.\n";
}