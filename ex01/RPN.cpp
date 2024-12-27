#include "RPN.hpp"

RPN::RPN() {}

RPN::~RPN() {}

RPN::RPN(const RPN& other) {
	nums = other.nums;
}

RPN& RPN::operator=(const RPN& other) {
	if (this != &other) {
		nums = other.nums;
	}
	return *this;
}

bool RPN::isOperator(std::string::iterator it) {
	return *it == '+' || *it == '-' || *it == '*' || *it == '/';
}

bool RPN::processRpn(const char *arg) {
	std::string tokens = std::string(arg);
	for (std::string::iterator it = tokens.begin(); it != tokens.end(); ++it) {
		skipSpaces(it);
		if (std::isdigit(*it))
			this->nums.push(*it - '0');
		else if (isOperator(it))
		{
			if (!calculate(it))
				throw std::runtime_error("Invalid expression");
		}
		else
		{
			throw std::runtime_error("Error: Invalid token");
		}
	}

	if (this->nums.size() != 1)
		throw std::runtime_error("Invalid expression: leftover numbers");

	std::cout << this->nums.top() << std::endl;
	return true;
}

void RPN::skipSpaces(std::string::iterator& it) {
	while (*it == ' ') {
		++it;
	}
}

bool RPN::calculate(std::string::iterator& it) {
	if (this->nums.size() < 2)
		return false;

	// 新規の数字
	int num1 =nums.top();
	nums.pop();
	// 今までの計算結果
	int num2 = nums.top();
	nums.pop();

	switch (*it)
	{
	case '+':
		nums.push(num2 + num1);
		break;
	case '-':
		nums.push(num2 - num1);
		break;
	case '*':
		nums.push(num2 * num1);
		break;
	case '/':
		if (num2 == 0)
			throw std::runtime_error("Division by zero");
		nums.push(num2 / num1);
		break;
	default:
		throw std::runtime_error("Invalid operator");
	}
	return true;
}