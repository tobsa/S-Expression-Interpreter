////////////////////////////////////////////////////////////////////////////////
// main.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include "Source/ExpressionParser.hpp"
#include "Source/SExpressionParser.hpp"
#include "Source/Evaluator.hpp"

////////////////////////////////////////////////////////////////////////////////
// Entry point of application
////////////////////////////////////////////////////////////////////////////////
int main()
{
	std::cout << "S-Expression Interpreter!" << std::endl << std::endl;

	while(true)
	{
		try
		{
			// Get input
			std::cout << "=> ";
			std::string line;
			getline(std::cin,line);

			if(line == "quit")
				break;
			else if(line.empty())
				throw std::string("Error: Empty expression");
			else
			{
				// Parse the expression
				if(line[0] == ':')
				{	
					SExpressionParser::print(SExpressionParser::parse(line.substr(1,line.size() - 1)));
				}
				else
				{
					std::cout << Evaluator::evaluate(ExpressionParser::parse(line));
				}
				
				std::cout << std::endl;
			}
		}
		catch(std::string s)
		{
			std::cout << s << std::endl;
		}
	}

	return 0;
}