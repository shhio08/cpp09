#pragma once

#include <string>
#include <stack>
#include <iostream>

class RPN {
private:
	bool isOperator(std::string::iterator it);
public:
	std::stack<int> nums;
	RPN();
	~RPN();
	RPN(const RPN& other);
	RPN& operator=(const RPN& other);

	bool calculate(std::string::iterator& it);
	bool processRpn(const char *arg);
	void skipSpaces(std::string::iterator& it);
};