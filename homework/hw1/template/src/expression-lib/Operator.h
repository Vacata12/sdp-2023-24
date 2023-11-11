#pragma once
#include <iostream>

class Operator
{
private:
	char symbol;
	char operation;
	unsigned priority = 0;
	char associativity;
public:
	Operator() = default;
	Operator(const char& symbol, const char& operation, const unsigned& priority, const char& associativity);
	Operator(const Operator& other) = default;
	Operator& operator=(const Operator& other) = default;
	~Operator() = default;

	void setSymbol(const char& symbol);
	void setOperation(const char& operation);
	void setPriority(const unsigned& priority);
	void setAssiciavity(const char& associativity);

	char getSymbol() const;
	char getOperation() const;
	unsigned getPriority() const;
	char getAssociativity() const;

	friend bool operator>(const Operator& ops1, const Operator& ops2);
	friend bool operator<(const Operator& ops1, const Operator& ops2);
	friend bool operator==(const Operator& ops1, const Operator& ops2);
	friend bool operator<=(const Operator& ops1, const Operator& ops2);
	friend bool operator>=(const Operator& ops1, const Operator& ops2);

	friend void swap(Operator& ops1, Operator& ops2);
};
