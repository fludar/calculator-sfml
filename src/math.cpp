#include "../include/math.h"

std::unordered_map<std::string, int> precedence = {
	{ "+", 1 },
	{ "-", 1 },
	{ "*", 2 },
	{ "/", 2 }
};

bool isNumber(const std::string& str) {
	if (str.empty()) return false;

	size_t start = 0;
	if (str[0] == '-') {
		if (str.length() == 1) return false; 
		start = 1;
	}

	bool hasDecimal = false;

	for (size_t i = start; i < str.length(); ++i) {
		if (str[i] == '.') {
			if (hasDecimal) return false; 
			hasDecimal = true;
		}
		else if (!std::isdigit(str[i])) {
			return false;
		}
	}

	return true;
}


std::vector<std::string> fnTokenize(const std::string& strCalcText)
{
	std::vector<std::string> vecTokens;
	std::string strNo;

	for (char chr : strCalcText) {
		if (isdigit(chr) || chr == '.') {
			strNo += chr;
		}
		else {
			if (!strNo.empty())
			{
				vecTokens.push_back(strNo);
				strNo.clear();
			}
			if (!isspace(chr)) {
				std::string strOp(1, chr);
				vecTokens.push_back(strOp);
			}
		}
	}
	if (!strNo.empty()) {
		vecTokens.push_back(strNo);
	}

	return vecTokens;
}

void math::fnShuntingYard(std::string& strCalcText)
{
	std::vector<std::string> vecTokens = fnTokenize(strCalcText);
	std::deque<std::string> outputQueue;
	std::deque<std::string> operatorStack;

	for (const auto& token : vecTokens) {
		if (isNumber(token)) {
			outputQueue.push_back(token);
		}
		else if (token == "(") {
			operatorStack.push_back(token);
		}
		else if (token == ")") {
			while (!operatorStack.empty() && operatorStack.back() != "(") {
				outputQueue.push_back(operatorStack.back());
				operatorStack.pop_back();
			}
			if (!operatorStack.empty() && operatorStack.back() == "(") {
				operatorStack.pop_back();
			}
		}
		else if (precedence.count(token)) {
			while (!operatorStack.empty() &&
				precedence.count(operatorStack.back()) &&
				precedence[operatorStack.back()] >= precedence[token]) {
				outputQueue.push_back(operatorStack.back());
				operatorStack.pop_back();
			}
			operatorStack.push_back(token);
		}

	}
	while (!operatorStack.empty()) {
		outputQueue.push_back(operatorStack.back());
		operatorStack.pop_back();
	}

	std::deque<double> evalStack;
	for (const auto& token : outputQueue) {
		if (isNumber(token)) {
			evalStack.push_back(std::stod(token));
		}
		else if (precedence.count(token)) {
			double b = evalStack.back(); evalStack.pop_back();
			double a = evalStack.back(); evalStack.pop_back();
			if (token == "+") evalStack.push_back(a + b);
			else if (token == "-") evalStack.push_back(a - b);
			else if (token == "*") evalStack.push_back(a * b);
			else if (token == "/") evalStack.push_back(a / b);
		}
	}

	if (!evalStack.empty()) {
		strCalcText = std::to_string(evalStack.back());
	}
	else {
		strCalcText.clear();
	}
}