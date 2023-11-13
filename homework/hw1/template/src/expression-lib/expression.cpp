#include "expression.h"
#include "DynamicArray.hpp"
#include "Operator.h"
#include <string>
#include<stack>



bool isDigit(const char& symbol);
bool isLetter(const char& symbol);
bool checkIfIsEmpty(const char* expression);
int checkBrackets(const char* expression, const unsigned& expLen);
void checkIfExpressionIsCorrect(const char* expression, DynamicArray<Operator>& opsArr, const unsigned& expLen);
double makeCalculation(const double& num1, const double& num2, const char& symbol);
void makeOperation(std::stack<double>& nums, const Operator& opsStack);
Operator findOperatorInArr(const char& symbol, const DynamicArray<Operator>& op);
void loadNumIntoTheStack(std::stack<double>& nums, const char* expression, unsigned& index, const unsigned& expLen);
// other function evaluate for brackets
double evaluate(const char* expression, unsigned& index, const unsigned& expLen, const DynamicArray<Operator>& opsArr);

//Load operators to the stack
void loadOperators(DynamicArray<Operator>& opsArr, std::istream& ops); 

///
/// @brief Evaluates an expression.
///
/// @param expression
///   A null-terminated string that contains the expression.
/// @param ops
///   An input stream which contains the descriptions of all operations used in the expression.
///
/// @return The calculated value of the expression
///
double evaluate(const char* expression, std::istream& ops)
{
	if (expression == nullptr)
		throw incorrect_expression("Incorrect expression\n");
	if (checkIfIsEmpty(expression))
		return 0.00;
	DynamicArray<Operator> opsArr;
	std::stack<Operator> opsStack;
	std::stack<double> nums;
	loadOperators(opsArr, ops);
	unsigned expLen = strlen(expression);
	checkIfExpressionIsCorrect(expression, opsArr, expLen);
	for (unsigned i = 0; i < expLen; i++)
	{
		if (expression[i] == ' ')
			continue;
		if (expression[i] == '(')
		{
			nums.push(evaluate(expression, i, expLen, opsArr));
		}
		if (isDigit(expression[i]))
			loadNumIntoTheStack(nums, expression, i, expLen);
		else if (expression[i] == '-')
			loadNumIntoTheStack(nums, expression, i, expLen);
		else if (isLetter(expression[i]))
		{
			Operator tmp2 = findOperatorInArr(expression[i], opsArr);
			if (opsStack.empty())
			{
				opsStack.push(tmp2);
				continue;
			}
			if (opsStack.top() < tmp2)
			{
				opsStack.push(tmp2);
			}
			else if (opsStack.top() > tmp2)
			{
				Operator tmp1;
				do
				{
					tmp1 = opsStack.top();
					makeOperation(nums, tmp1);
					opsStack.pop();
					if (opsStack.empty())
						break;
				} while (opsStack.top() >= tmp2);
				opsStack.push(tmp2);
			}
			else if (opsStack.top() == tmp2)
			{
				if (opsStack.top().getAssociativity() == 'L')
				{
					Operator tmp = opsStack.top();
					makeOperation(nums, tmp);
					opsStack.pop();
					opsStack.push(tmp2);
				}
				else if (tmp2.getAssociativity() == 'R')
				{
					i += 2;
					if (expression[i] == '(')
					{
						nums.push(evaluate(expression, i, expLen, opsArr));
					}
					else
					{
						loadNumIntoTheStack(nums, expression, i, expLen);
					}
					makeOperation(nums, tmp2);
				}
			}
		}
	}
	while (!opsStack.empty())
	{
		Operator tmp = opsStack.top();
		opsStack.pop();
		makeOperation(nums, tmp);
	}
	double result = nums.top();
	std::cout << "Result: " << result << std::endl;
	return result;
}

//helping functions
bool isDigit(const char& symbol)
{
	return '0' <= symbol && symbol <= '9';
}
bool isLetter(const char& symbol)
{
	return 'a' <= symbol && symbol <= 'z';
}
bool checkIfIsEmpty(const char* expression)
{

	std::string tmp = expression;
	tmp.erase(remove(tmp.begin(), tmp.end(), ' '), tmp.end());
	return tmp.empty();
}
int checkBrackets(const char* expression, const unsigned& expLen)
{
	int result = 0;
	for (size_t i = 0; i < expLen; i++)
	{
		if (expression[i] == '(')
			result += 1;
		else if (expression[i] == ')')
			result -= 1;
	}
	return result;
}
void checkIfExpressionIsCorrect(const char* expression, DynamicArray<Operator>& opsArr, const unsigned& expLen)
{
	if (isLetter(expression[0]))
		throw incorrect_expression("Symbol at first possition!\n");
	if (isLetter(expression[expLen - 1]))
		throw incorrect_expression("Symbol at last possition!\n");
	if(checkBrackets(expression, expLen) != 0)
		throw incorrect_expression("Worng brackets!\n");

	bool noNums = true;
	for (size_t i = 0; i < expLen; i++)
	{
		if (i + 1 < expLen && expression[i] == '-' && !isDigit(expression[i + 1]))
			throw incorrect_expression("Minus as operator!\n");
		else if (i - 2 < 0)
			continue;
		else if (isLetter(expression[i]) && isLetter(expression[i - 2]))
			throw incorrect_expression("Two consecutive symbols!\n");
		else if (isDigit(expression[i]) && isDigit(expression[i - 2]) && expression[i - 1] == ' ')
			throw incorrect_expression("Two consecutive numbers!\n");
		else if (isLetter(expression[i]))
			findOperatorInArr(expression[i], opsArr);
		else if (noNums && isDigit(expression[i]))
			noNums = false;
		else if (expression[i] == ')' && expression[i - 2] == ')')
			continue;
		else if (!isDigit(expression[i]) && !isLetter(expression[i])
			&& expression[i] != '-' && expression[i] != ' '
			&& expression[i] != '(' && expression[i] != ')')
			throw incorrect_expression("Wrong symbol in expression!\n");
		else if (isLetter(expression[i]) && noNums == true)
			throw incorrect_expression("Only symbols!\n");
		else if (expression[i] == '(' && expression[i + 1] != ' '
			|| expression[i - 1] != ' ' && expression[i] == ')')
			throw incorrect_expression("Before and after closing and opening brackets should have whitespace!\n");
		else if (isLetter(expression[i]) && !isDigit(expression[i + 2]))
			throw incorrect_expression("No num after symbol!\n");
		else if (expression[i] == '(' && expression[i + 2] == '-')
			continue;
		else if (expression[i] == '(' && !isDigit(expression[i + 2])
			|| expression[i] == ')' && !isDigit(expression[i - 2]))
			throw incorrect_expression("Before and after closing and opening brackets should have number!\n");
	}
	if (noNums)
		throw incorrect_expression("No nums");
}
double makeCalculation(const double& num1, const double& num2, const char& symbol)
{
	switch (symbol)
	{
	case '+':
		return num1 + num2;
	case '-':
		return num1 - num2;
	case '*':
		return num1 * num2;
	case '/':
		return num1 / num2;
	}
}
void makeOperation(std::stack<double>& nums, const Operator& op)
{
	double num2 = nums.top();
	nums.pop();
	double num1 = nums.top();
	nums.pop();
	nums.push(makeCalculation(num1, num2, op.getOperation()));
}
Operator findOperatorInArr(const char& symbol, const DynamicArray<Operator>& opsArr)
{
	unsigned size = opsArr.getSize();
	for (size_t i = 0; i < size; i++)
	{
		if (symbol == opsArr[i].getSymbol())
			return opsArr[i];
	}
	throw incorrect_expression("Error: There is no such operator in the input!\n");
}
void loadNumIntoTheStack(std::stack<double>& nums, const char* expression, unsigned& index, const unsigned& expLen)
{
	bool negative = false;
	if (expression[index] == '-')
	{
		negative = true;
		index++;
	}
	std::string helper;
	unsigned i = 0;
	while (isDigit(expression[index]) != false)
	{
		helper += expression[index];
		if ((index + 1) == expLen)
		{
			index++;
			break;
		}
		index++;
	}
	if (helper.empty())
	{
		return;
	}
	double num = std::stod(helper);
	if (negative)
		num *= -1;
	nums.push(num);
}
double evaluate(const char* expression, unsigned& i, const unsigned& expLen, const DynamicArray<Operator>& opsArr)
{
	i++;
	std::stack<Operator> opsStack;
	std::stack<double> nums;
	while (expression[i] != ')')
	{
		if (expression[i] == ' ')
		{
			i++;
			continue;
		}
		if (expression[i] == '(')
		{
			nums.push(evaluate(expression, i, expLen, opsArr));
		}
		if (isDigit(expression[i]))
			loadNumIntoTheStack(nums, expression, i, expLen);
		else if (expression[i] == '-')
			loadNumIntoTheStack(nums, expression, i, expLen);
		else if (isLetter(expression[i]))
		{
			Operator tmp2 = findOperatorInArr(expression[i], opsArr);
			if (opsStack.empty())
			{
				opsStack.push(tmp2);
				i++;
				continue;
			}
			if (opsStack.top() < tmp2)
			{
				opsStack.push(tmp2);
			}
			else if (opsStack.top() > tmp2)
			{
				Operator tmp1;
				do
				{
					tmp1 = opsStack.top();
					makeOperation(nums, tmp1);
					opsStack.pop();
					if (opsStack.empty())
						break;
				} while (opsStack.top() >= tmp2);
				opsStack.push(tmp2);
			}
			else if (opsStack.top() == tmp2)
			{
				if (opsStack.top().getAssociativity() == 'L')
				{
					Operator tmp = opsStack.top();
					makeOperation(nums, tmp);
					opsStack.pop();
					opsStack.push(tmp2);
				}
				else if(tmp2.getAssociativity() == 'R')
				{
					i += 2;
					if (expression[i] == '(')
					{
						nums.push(evaluate(expression, i, expLen, opsArr));
					}
					else
					{
						loadNumIntoTheStack(nums, expression, i, expLen);
					}
					makeOperation(nums, tmp2);
				}
			}
		}
		i++;
	}
	while (!opsStack.empty())
	{
		Operator tmp = opsStack.top();
		opsStack.pop();
		makeOperation(nums, tmp);
	}
	double result = nums.top();
	return result;
}


void loadOperators(DynamicArray<Operator>& opsArr, std::istream& ops)
{
	char helper;
	unsigned priorityHelper = 0;
	Operator opsHelper;
	short count = 1;
	while ((helper = ops.get()) != EOF)
	{
		if (helper == '\n' || helper == ' ')
			continue;
		switch (count)
		{
		case 1:
			opsHelper.setSymbol(helper);
			count += 1;
			break;
		case 2:
			opsHelper.setOperation(helper);
			count += 1;
			break;
		case 3:
			while (isDigit(helper))
			{
				priorityHelper = (priorityHelper * 10) + (helper - '0');
				helper = ops.get();
			}
			opsHelper.setPriority(priorityHelper);
			priorityHelper = 0;
			count += 1;
			break;
		case 4:
			opsHelper.setAssiciavity(helper);
			opsArr.pushBack(opsHelper);
			count = 1;
			break;
		}
	}
}
