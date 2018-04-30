#include "header\StringManp.h"

std::string removeBrackets(std::string line, char b1, char b2);

//Function to return precedence of operators
bool isOperator(char c) {
	if (c == '*' || c == '+')
		return true;
	else if (c == '-' || c == ',')
		return true;
	else if (c == '(' || c == ')')
		return true;
	else if (c == '|')
		return true;
	else
		return false;
}

//check the existance of braces and remove them from the string
std::string removeBraces(std::string line) {
	return removeBrackets(line, '{', '}');
}

std::string removeSBrackets(std::string line) {
	return removeBrackets(line, '[', ']');
}

std::string removeBrackets(std::string line, char b1, char b2) {
	int a, b, n = line.length();

	//escape spaces at start
	for (a = 0; a < n; a++) {
		if (line[a] != ' ')
			break;
	}

	if (line[a] != b1) {
		std::cout << "remove braces error: no braces";
		return NULL;
	}

	//escape spaces at end
	for (b = n - 1; b >= a; b--) {
		if (line[b] != ' ')
			break;
	}

	if (line[b] != b2 || a == b) {
		std::cout << "remove braces error: only one brace";
		return NULL;
	}

	std::cout << a << " " << b << "\n";

	return line.substr(a + 1, b - a - 1);
}

std::string trim(std::string line) {

	int a, b, n = line.length();

	//escape spaces at start
	for (a = 0; a < n; a++) {
		if (line[a] != ' ')
			break;
	}

	if (a == n - 1 && a != 0) {
		std::cout << "trim string error: empty string";
		return NULL;
	}

	//escape spaces at end
	for (b = n - 1; b >= a; b--) {
		if (line[b] != ' ')
			break;
	}

	return substring(line, a, b);

}

std::string substring(std::string line, int from, int to) {
	return line.substr(from, to - from + 1);
}

//get the first word untill : ir =
int getLHSIndex(std::string line) {

	unsigned i;
	for (i = 0; i < line.length(); i++) {
		if (line[i] == ':' || line[i] == '=') {
			break;
		}
	}

	if (i == line.length() - 1) {
		std::cout << "format doesn't match";
		return -1;
	}

	return i;
}

Operand getOperand(std::string line, int index) {
	unsigned i;
	std::string op;
	if (line[index] != '\\') {
		if(line[index + 1] == 'L') {
			return Operand(index + 2, Epthnon);
		}
		op.push_back(line[index++]);
	}

	for (i = index; i < line.length(); i++) {
		if (line[i] == ' ' || isOperator(line[i])) {
			break;
		}
		if (line[i] == '\\') {
			op.push_back(line[i + 1]);
			i++;
		} else {
			op.push_back(line[i]);
		}
	}

	std::cout<< line << " i = " << i - 1 << line[i - 1];

	return Operand(i - 1, op);
}

