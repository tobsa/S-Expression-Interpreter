////////////////////////////////////////////////////////////////////////////////
// SExpressionParser.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include "SExpressionParser.hpp"
#include "Scanner.hpp"
#include <iostream>

bool SExpressionParser::m_openParantheses = false;
int	 SExpressionParser::m_numParantheses  = 0;

////////////////////////////////////////////////////////////////////////////////
SExpressionParser::SExpressionPtr SExpressionParser::parse(const std::string& line)
{
	Scanner scanner(line);

	m_openParantheses = false;
	m_numParantheses  = 0;

	SExpressionPtr sx = readSExpression(scanner);

	if(scanner.hasToken())
		throw std::string("Error: Unexpected " + scanner.getToken());
	
	return sx;
}

////////////////////////////////////////////////////////////////////////////////
SExpressionParser::SExpressionPtr SExpressionParser::readSExpression(Scanner& scanner)
{
	SExpressionPtr sx = readTerm(scanner);

	if(!m_openParantheses)
		throw std::string("Error: Expression must begin with a (");

	if(scanner.peekToken() == ")")
	{
		sx = new SExpression(sx,NULL);

		std::string token = scanner.getToken();
		if(scanner.hasToken() && (m_numParantheses - 1) == 0)
			throw std::string("Error: Invalid s-expression");
		scanner.saveToken(token);

		return sx;
	}
	else if(scanner.peekToken() != "")
	{
		sx = new SExpression(sx,readSExpression(scanner));
		return sx;
	}
	else
	{
		return sx;
	}
}

////////////////////////////////////////////////////////////////////////////////
SExpressionParser::SExpressionPtr SExpressionParser::readTerm(Scanner& scanner)
{
	SExpressionPtr sx;
	const std::string token = scanner.getToken();

	if(token == "(")
	{
		m_openParantheses = true;
		m_numParantheses++;

		sx = readSExpression(scanner);

		if(scanner.getToken() != ")")
			throw std::string("Error: Unbalanced parantheses");
		m_numParantheses--;
	}
	else if(isdigit(token[0]))
	{
		sx = new SExpression(SExpression::stringToInteger(token));
	}
	else if(isalpha(token[0]))
	{
		sx = SExpression::stringToAtom(token);
	}
	else if(token == ")")
	{
		sx = new SExpression();
		scanner.saveToken(token);
	}
	else
	{
		throw std::string("Error: Invalid token: " + token);
	}

	return sx;
}

////////////////////////////////////////////////////////////////////////////////
void SExpressionParser::print(SExpressionPtr sx)
{
	std::cout << "(";
	printSExpression(sx);
}

////////////////////////////////////////////////////////////////////////////////
void SExpressionParser::printSExpression(SExpressionPtr sx)
{
	if(!sx) return;

	if(sx->getType() == SExpression::Number)
		std::cout << sx->getNumber() << "";
	else if(sx->getType() == SExpression::Atom)
		std::cout << sx->getAtom() << "";
	else if(sx->getType() == SExpression::DottedPair)
	{
		if(sx->getCar()->getType() == SExpression::DottedPair) 
			std::cout << "(";

		printSExpression(sx->getCar());

		if(sx->getCar()->getType() != SExpression::DottedPair && sx->getCdr() != NULL)
			std::cout << " ";

		printSExpression(sx->getCdr());

		if(sx->getCdr() == NULL)
			std::cout << ")";
	}
}



