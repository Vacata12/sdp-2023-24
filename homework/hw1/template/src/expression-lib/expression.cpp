#include "expression.h"
#include "Stack.hpp"
#include "DynamicArray.hpp"
#include "Operator.h"
#include <string>


//Helping functions
bool isDigit(const char& symbol);
bool isLetter(const char& symbol);
bool checkIfIsEmpty(const char* expression);
int checkBrackets(const char* expression, const unsigned& expLen);
void checkIfExpressionIsCorrect(const char* expression, DynamicArray<Operator>& opsArr, const unsigned& expLen);
double makeCalculation(const double& num1, const double& num2, const char& symbol);
void makeOperation(Stack<double>& nums, Stack<Operator>& opsStack);
char* expressionInBrackets(const char* expression, unsigned& index, const unsigned& expLen);
Operator findOperatorInArr(const char& symbol, const DynamicArray<Operator>& opsArr);
void loadNumIntoTheStack(Stack<double>& nums, const char* expression, unsigned& index, const unsigned& expLen);
double evaluateInBrackets(const char* expression, const DynamicArray<Operator>& opsArr);

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
	Stack<Operator> opsStack;
	Stack<double> nums;
	loadOperators(opsArr, ops);
	unsigned expLen = strlen(expression);
	checkIfExpressionIsCorrect(expression, opsArr, expLen);
	for (unsigned i = 0; i < expLen; i++)
	{
		if (expression[i] == '(')
		{
			nums.push(evaluateInBrackets(expressionInBrackets(expression, i, expLen), opsArr));
		}
		if (i + 1 < expLen)
		{
			if (expression[i] == '-' && isDigit(expression[i + 1]))
			{
				loadNumIntoTheStack(nums, expression, i, expLen);
			}
		}
		if (isDigit(expression[i]))
		{
			loadNumIntoTheStack(nums, expression, i, expLen);
		}
		else if (isLetter(expression[i]))
		{
			if (!opsStack.isEmpty())
			{
				if (opsStack.peek() == findOperatorInArr(expression[i], opsArr))
				{
					makeOperation(nums, opsStack);
				}
			}
			opsStack.push(findOperatorInArr(expression[i], opsArr));
		}
		if (opsStack.getCurrSize() > 1)
		{
			if (opsStack[0] < opsStack[1] && isDigit(expression[i]))
			{
				loadNumIntoTheStack(nums, expression, i, expLen);
				makeOperation(nums, opsStack);
			}
			else
			{
				makeOperation(nums, opsStack);
			}
		}
	}
	if (nums.getCurrSize() == 1 && opsStack.getCurrSize() == 0)
	{
		double result = nums.peek();
		return result;
	}
	if(nums.getCurrSize() == 2 && opsStack.getCurrSize() == 1)
		makeOperation(nums, opsStack);
	//throw std::exception();
	double result = nums.peek();
	std::cout << "Result: " << nums[0] << std::endl;
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
	const char error[] = "Incorrect Expression!\n";
	if (isLetter(expression[0]))
		throw incorrect_expression(error);
	if (isLetter(expression[expLen - 1]))
		throw incorrect_expression(error);


	bool noNums = true;
	for (size_t i = 0; i < expLen; i++)
	{
		if (i + 1 < expLen && expression[i] == '-' && !isDigit(expression[i + 1]))
			throw incorrect_expression(error);
		else if (i - 2 < 0)
			continue;
		else if (isLetter(expression[i]) && isLetter(expression[i - 2]))
			throw incorrect_expression(error);
		else if (isDigit(expression[i]) && isDigit(expression[i - 2]))
			throw incorrect_expression(error);
		else if (isLetter(expression[i]))
			findOperatorInArr(expression[i], opsArr);
		else if (noNums && isDigit(expression[i]))
			noNums = false;
		else if (checkBrackets(expression, expLen) != 0)
			throw incorrect_expression(error);
		else if (!isDigit(expression[i]) && !isLetter(expression[i])
			&& expression[i] != '-' && expression[i] != ' '
			&& expression[i] != '(' && expression[i] != ')')
			throw incorrect_expression(error);
		else if (isLetter(expression[i]) && noNums == true)
			throw incorrect_expression(error);
		else if (expression[i] == '(' && expression[i + 1] != ' '
			|| expression[i - 1] != ' ' && expression[i] == ')')
			throw incorrect_expression(error);
		else if (isLetter(expression[i]) && !isDigit(expression[i + 2]))
			throw incorrect_expression(error);
		else if (expression[i] == '(' && expression[i + 2] == '-')
			continue;
		else if (expression[i] == '(' && !isDigit(expression[i + 2])
			|| expression[i] == ')' && !isDigit(expression[i - 2]))
			throw incorrect_expression(error);
	}
	if (noNums)
		throw incorrect_expression(error);
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
void makeOperation(Stack<double>& nums, Stack<Operator>& opsStack)
{
	double num2 = nums.peek();
	nums.pop();
	double num1 = nums.peek();
	nums.pop();
	Operator tmp = opsStack.peek();
	opsStack.pop();
	nums.push(makeCalculation(num1, num2, tmp.getOperation()));
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
void loadNumIntoTheStack(Stack<double>& nums, const char* expression, unsigned& index, const unsigned& expLen)
{
	bool negative = false;
	double num = 0;
	if (expression[index] == '-')
	{
		negative = true;
		index++;
	}
	while (expression[index] != ' ')
	{
		num = (num * 10) + (expression[index] - '0');
		if ((index + 1) == expLen)
		{
			index++;
			break;
		}
		index++;
	}
	if (negative)
		num *= -1;
	nums.push(num);
}
char* expressionInBrackets(const char* expression, unsigned& index, const unsigned& expLen)
{
	index += 2;
	unsigned count = 0;
	unsigned indexTmp = index;
	while (expression[indexTmp] != ')')
	{
		indexTmp += 1;
		count += 1;
	}
	char* result = new char[count];
	count = 0;
	while (index != (indexTmp - 1))
	{
		result[count++] = expression[index++];
	}
	result[count] = '\0';
	return result;
}
double evaluateInBrackets(const char* expression, const DynamicArray<Operator>& opsArr)
{
	Stack<Operator> opsStack;
	Stack<double> nums;
	unsigned expLen = strlen(expression);
	for (unsigned i = 0; i < expLen; i++)
	{
		if (expression[i] == '(')
		{
			nums.push(evaluateInBrackets(expressionInBrackets(expression, i, expLen), opsArr));
		}
		if (i + 1 < expLen)
		{
			if (expression[i] == '-' && isDigit(expression[i + 1]))
			{
				loadNumIntoTheStack(nums, expression, i, expLen);
			}
		}
		if (isDigit(expression[i]))
		{
			loadNumIntoTheStack(nums, expression, i, expLen);
		}
		else if (isLetter(expression[i]))
		{
			if (!opsStack.isEmpty())
			{
				if (opsStack.peek() == findOperatorInArr(expression[i], opsArr))
				{
					makeOperation(nums, opsStack);
				}
			}
			opsStack.push(findOperatorInArr(expression[i], opsArr));
		}
		if (opsStack.getCurrSize() > 1)
		{
			if (opsStack[0] < opsStack[1] && isDigit(expression[i]))
			{
				loadNumIntoTheStack(nums, expression, i, expLen);
				makeOperation(nums, opsStack);
			}
			else
			{
				makeOperation(nums, opsStack);
			}
		}
	}
	if (nums.getCurrSize() == 1 && opsStack.getCurrSize() == 0)
	{
		double result = nums.peek();
		return result;
	}
	if (nums.getCurrSize() == 2 && opsStack.getCurrSize() == 1)
		makeOperation(nums, opsStack);
	//throw std::exception();
	double result = nums.peek();
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
