#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cctype>

class RPN {
private:
	std::stack<std::string> tokens;

public:
	RPN(const std::string &expression);
	RPN(const RPN &other);
	RPN &operator=(const RPN &other);
	~RPN();

	void evaluate();
};

#endif
