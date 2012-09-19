////////////////////////////////////////////////////////////////////////////////
// ExpressionParser.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include "ExpressionParser.hpp"
#include "Scanner.hpp"

////////////////////////////////////////////////////////////////////////////////
ExpressionParser::SExpressionPtr ExpressionParser::parse(const std::string& line)
{
	Scanner scanner(line);

	SExpressionPtr sx = readExpression(scanner,0);

	if(scanner.hasToken())
		throw std::string("Error: Unexpected: " + scanner.getToken());

	return sx;
}
	
////////////////////////////////////////////////////////////////////////////////
ExpressionParser::SExpressionPtr ExpressionParser::readExpression(Scanner& scanner, int precedence)
{
	SExpressionPtr sx = readTerm(scanner);
	
	while(true)
	{
		std::string token  = scanner.getToken();
		int newPrecedence  = getPrecedence(token);

		if(newPrecedence <= precedence) 
		{
			scanner.saveToken(token);
			break;
		}

		sx = new SExpression(SExpression::stringToAtom(token),new SExpression(sx,new SExpression(readExpression(scanner,newPrecedence),NULL)));
	}
	
	return sx;	
}

////////////////////////////////////////////////////////////////////////////////
ExpressionParser::SExpressionPtr ExpressionParser::readTerm(Scanner& scanner)
{
	SExpressionPtr sx;
	std::string token = scanner.getToken();
	
	if(token == "(")
	{
		sx = readExpression(scanner,0);

		if (scanner.getToken() != ")")
			throw std::string("Error: Unbalanced parantheses");
	}
	else if(isdigit(token[0]))
	{
		sx = new SExpression(SExpression::stringToInteger(token));
	}
	else if(isalpha(token[0]))
	{
		sx = SExpression::stringToAtom(token);
	}
	else
	{
		throw std::string("Invalid token: " + token);
	}

	return sx;
}

////////////////////////////////////////////////////////////////////////////////
int ExpressionParser::getPrecedence(const std::string& token)
{
	if(token.size() > 1) return 0;

	switch(token[0])
	{
		case '=': return 1;
		case '+': return 2;
		case '-': return 2;
		case '*': return 3;
		case '/': return 3;
		case '^': return 4;
		default : return 0;
	}
}