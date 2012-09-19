////////////////////////////////////////////////////////////////////////////////
// Evaluator.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include "Evaluator.hpp"

Evaluator::VariableMap Evaluator::m_variables;

////////////////////////////////////////////////////////////////////////////////
int Evaluator::evaluate(SExpressionPtr sx)
{
	switch(sx->getType())
	{
		case SExpression::Number    : return sx->getNumber();
		case SExpression::Atom      : return atomToInteger(sx->getAtom());
		case SExpression::DottedPair: return evaluateDottedPair(sx);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
int Evaluator::atomToInteger(const std::string& atom)
{
	if(m_variables.find(atom) == m_variables.end())
	{
		throw std::string("Error: " + atom + " is undefined");
	}

	return m_variables[atom];
}

////////////////////////////////////////////////////////////////////////////////
int Evaluator::evaluateDottedPair(SExpressionPtr sx)
{
	if(!sx) return 0;
	
	if(sx->getCar()->getType() == SExpression::Number)
	{
		return evaluate(sx->getCar());
	}
	else if(sx->getCar()->getType() == SExpression::Atom)
	{
		std::string token = sx->getCar()->getAtom();

		if(!isOperator(token))
			return evaluate(sx->getCar());

		if(token == "=")
		{
			std::string atom = sx->getCdr()->getCar()->getAtom();
			int value        = evaluate(sx->getCdr()->getCdr());

			if(m_variables.find(atom) == m_variables.end())
				m_variables.insert(VariablePair(atom,value));
			else
				m_variables[atom] = value;
			return value;
		}

		int lhs = evaluateDottedPair(sx->getCdr());
		int rhs = evaluateDottedPair(sx->getCdr()->getCdr());

		if(token == "+")      return lhs + rhs;
		else if(token == "-") return lhs - rhs;
		else if(token == "*") return lhs * rhs;
		else if(token == "^") return power(lhs,rhs);
		else if(token == "/") 
		{
			if(rhs == 0)
				throw std::string("Error: Can't divide by zero");
			return lhs / rhs;
		}
		else
			throw std::string("Error: Invalid operator");
	}
	else if(sx->getCar()->getType() == SExpression::DottedPair)
		return evaluateDottedPair(sx->getCar());
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool Evaluator::isOperator(const std::string& token)
{
	if(token == "=" || token == "+" || token == "-" || token == "*" || token == "/" || token == "^")
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////
int Evaluator::power(int lhs,int rhs)
{
	int sum = 1;
	for(int i = 0; i < rhs; i++)
		sum *= lhs;
	return sum;
}