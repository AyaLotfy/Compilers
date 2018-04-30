/*
 * Expression.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#ifndef HEADER_EXPRESSION_H_
#define HEADER_EXPRESSION_H_

#include "FileScanner.h"
#include "Operand.h"

using namespace std;

class Expression {

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
	std::vector<int> createSM (std::string operand);
	void infixToPostfix();
	int prec(char c);

public:
	stack<vector<int>> stateStack;
	std::string s;

	Expression(string s);
	vector<int> evalPostfix();
};

#endif /* HEADER_EXPRESSION_H_ */
