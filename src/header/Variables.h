/*
 * InfixToPostfix.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#ifndef HEADER_VARIABLES_H_
#define HEADER_VARIABLES_H_

#include "FileScanner.h"

#include <stack>
#include <vector>
#include <string>

using namespace std;

class Variables {

private:
	vector<int> createVector(int a, int b);
	void invertStack();
	std::vector<int> addOperand(std::string operand);
	std::vector<int> addOperand(int);
	vector<int> OR(int op1, int op2, int op3, int op4);
	vector<int> AND(int op1, int op2, int op3, int op4);
	vector<int> DASH(int op1, int op2, int op3, int op4);
	vector<int> Closure(int op1, int op2, int op3, int op4);
	vector<int> PClosure(int op1, int op2, int op3, int op4);
	vector<int> eval(int op1, int op2, int op3, int op4, vector<int> operate);
	std::vector<int> createSM(std::string operand);
	int prec(char c);

public:
	stack<vector<int>> stateStack;
	std::vector<std::vector<std::vector<int>>> SM;
	std::string s;
	int start, end;

	vector<int> evalPostfix();
	Variables(string s);
	void infixToPostfix();
};

#endif /* HEADER_VARIABLES_H_ */
