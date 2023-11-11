#include "Operator.h"

Operator::Operator(const char& symbol, const char& operation, const unsigned& priority, const char& associativity)
{
	setSymbol(symbol);
	setOperation(operation);
	setPriority(priority);
	setAssiciavity(associativity);
}

void Operator::setSymbol(const char& symbol)
{
	if (('a' <= symbol && symbol <= 'z') || ('A' <= symbol && symbol <= 'Z'))
	{
		if ('A' < symbol && symbol < 'Z')
			this->symbol = symbol + ('a' - 'A');
		else
			this->symbol = symbol;
	}
	else
		throw std::invalid_argument("Symbol must be from English alphabet!\n");
}
void Operator::setOperation(const char& operation)
{
	if (operation != '+' && operation != '-' && operation != '*' && operation != '/')
	{
		throw std::invalid_argument("Invalid operation!\n");
	}
	this->operation = operation;
}
void Operator::setPriority(const unsigned& priority)
{
	this->priority = priority;
}
void Operator::setAssiciavity(const char& associativity)
{
	if (associativity == 'L' || associativity == 'R')
	{
		this->associativity = associativity;
		return;
	}
	throw std::invalid_argument("Invalid associativity!\n");
}

char Operator::getSymbol() const
{
	return symbol;
}
char Operator::getOperation() const
{
	return operation;
}
unsigned Operator::getPriority() const
{
	return priority;
}
char Operator::getAssociativity() const
{
	return associativity;
}

bool operator>(const Operator& ops1, const Operator& ops2)
{
	return ops1.getPriority() > ops2.getPriority();
}
bool operator<(const Operator& ops1, const Operator& ops2)
{
	return ops1.getPriority() < ops2.getPriority();
}
bool operator==(const Operator& ops1, const Operator& ops2)
{
	return ops1.getPriority() == ops2.getPriority();
}
bool operator<=(const Operator& ops1, const Operator& ops2)
{
	return ops1 < ops2 || ops1 == ops2;	
}
bool operator>=(const Operator& ops1, const Operator& ops2)
{
	return ops1 > ops2 || ops1 == ops2;
}

void swap(Operator& ops1, Operator& ops2)
{
	Operator tmp = ops1;
	ops1 = ops2;
	ops2 = tmp;
}