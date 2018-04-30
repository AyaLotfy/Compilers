/* C++ implementation to convert infix expression to postfix*/
// Note that here we use std::stack  for Stack operations
#include<bits/stdc++.h>

#include "header/Variables.h"

#include "header/FileScanner.h"

using namespace std;

Variables::Variables(string s) {
	this->SM = std::vector<std::vector<std::vector<int>>>();
	this->s = s;

	infixToPostfix();
}

//Function to return precedence of operators
int Variables::prec(char c) {
	if (c == '*' || c == '+')
		return 3;
	else if (c == '-' || c == ',')
		return 2;
	else if (c == '|')
		return 1;
	else
		return -1;
}

//The main function to convert infix expression
//to postfix expression
void Variables::infixToPostfix() {

	std::cout << s << "\n";

	stack<std::string> st;

	int l = s.length();
	string ns;

	for (int i = 0; i < l; i++) {

		if (s[i] == ' ') {
			continue;
		}

		std::cout << "loop i = " << i << "size = " << SM.size() << endl;

		// If the scanned character is an �(�, push it to the stack.
		if (s[i] == '(') {
			st.push(",");
			st.push("(");
		}

		// If the scanned character is an �)�, pop and to output string from the stack
		// until an �(� is encountered.
		else if (s[i] == ')') {
			while (!st.empty() && st.top() != "(") {
				std::string c = st.top();
				st.pop();

				stateStack.push(createVector(-1, (int) (c[0])));
				ns += c;
			}
			if (st.top() == "(") {
				std::string c = st.top();
				st.pop();
			}
		}

		// If the scanned character is an operand, add it to output string.
		else if (s[i] != ' ' && prec(s[i]) == -1) {
			Operand e = getOperand(s, i);
			std::string operand = e.op;

			stateStack.push(addOperand(operand));

			//std::cout << "operand " << operand << SM.size() << " \n";
			ns += operand;
			i = e.index;
		}

		//If an operator is scanned
		else {
			while (!st.empty() && prec(s[i]) <= prec(st.top()[0])) {
				std::string c = st.top();
				st.pop();

				stateStack.push(createVector(-1, (int) (c[0])));
				ns += c;
			}
			std::string ss;
			ss.push_back(s[i]);
			std::cout << "operator " << ss << " \n";
			st.push(ss);
		}

	}

	//Pop all the remaining elements from the stack
	while (!st.empty()) {
		std::string c = st.top();
		st.pop();

		stateStack.push(createVector(-1, (int) (c[0])));
		ns += c;
	}

	cout << ns << endl;

	cout << stateStack.size() << endl;

	invertStack();
}

std::vector<int> Variables::addOperand(std::string operand) {

	std::vector<int> startEnd;
	std::cout << "here add operand" << operand << endl;

	//one character
	if (operand.length() == 1) {

		int stateNum = SM.size();
		startEnd.push_back(stateNum);
		startEnd.push_back(stateNum + 1);

		//create start state
		std::vector<int> v = std::vector<int>();
		v.push_back(operand[0]);
		v.push_back(stateNum + 1);
		std::vector<std::vector<int>> vv;
		vv.push_back(v);
		SM.push_back(vv);

		//create end state
		SM.push_back(std::vector<std::vector<int>>());

		return startEnd;

	} else {

		for (unsigned i = 0; i < variableName.size(); i++) {
			if (variableName[i].compare(operand) == 0) {
				return addOperand(i);
			}
		}

		return createSM(operand);

		cout << "error variable not found" << endl;
	}

	return startEnd;
}

std::vector<int> Variables::createSM(std::string operand) {

	cout << "create SM operand : " << operand << endl;

	std::vector<int> startEnd;

	int stateNum = SM.size();
	startEnd.push_back(stateNum);
	startEnd.push_back(stateNum + operand.length());

	unsigned i;

	SM.push_back(std::vector<std::vector<int>>());

	for (i = 0; i < operand.length(); i++) {
		std::vector<int> v = std::vector<int>();
		v.push_back(operand[i]);
		v.push_back(stateNum + i + 1);
		SM[stateNum + i].push_back(v);


		SM.push_back(std::vector<std::vector<int>>());
	}

	return startEnd;
}


std::vector<int> Variables::addOperand(int index) {

	int shift = SM.size();

	unsigned i;
	for (i = 0; i < variables[index].size(); i++) {
		SM.push_back(std::vector<std::vector<int>>());

		for (unsigned j = 0; j < variables[index][i].size(); j++) {
			vector<int> v = createVector(variables[index][i][j][0],
					variables[index][i][j][1] + shift);
			SM[shift + i].push_back(v);
		}
	}

	return createVector(variableStartEnd[index][0] + shift,
			variableStartEnd[index][1] + shift);
}

void Variables::invertStack() {

	std::stack<vector<int>> st2;

	std::cout << "stack" << endl;

	while (!stateStack.empty()) {
		if (stateStack.top().empty()) {
			std::cout << "empty slot \n";
		} else if (stateStack.top()[0] == -1) {
			std::cout << (char) stateStack.top()[1] << "\n";
		} else {
			std::cout << stateStack.top()[0] << " " << stateStack.top()[1]
					<< endl;
		}
		st2.push(stateStack.top());
		stateStack.pop();

	}

	stateStack = st2;
}

//create int vector then push a and b
std::vector<int> Variables::createVector(int a, int b) {
	std::vector<int> v;
	v.push_back(a);
	v.push_back(b);
	return v;
}

// returns the value when a specific operator
// operates on two operands
vector<int> Variables::eval(int op1, int op2, int op3, int op4,
		vector<int> operate) {

	std::cout << "evaluation " << op1 << endl;

	switch ((char) operate[1]) {
	case '|':
		return OR(op1, op2, op3, op4);
	case ',':
		return AND(op1, op2, op3, op4);
	case '-':
		return DASH(op1, op2, op3, op4);
	case '*':
		return Closure(op1, op2, op3, op4);
	case '+':
		return PClosure(op1, op2, op3, op4);
	default:
		return vector<int>();
	}
}

// evaluates the postfix operation
// this module neither supports multiple digit integers
// nor looks for valid expression
// However it can be easily modified and some additional
// code can be added to overcome the above mentioned limitations
// it's a simple function which implements the basic logic to
// evaluate postfix operations using stack
vector<int> Variables::evalPostfix() {

	stack<vector<int>> s;
	vector<int> ch;
	vector<int> val;
	while (!stateStack.empty()) {
		ch = stateStack.top();
		if (ch[0] != -1) {
			// we saw an operand
			// push it to stack
			s.push(ch);
		} else {
			// we saw an operator
			// pop off the top two operands from the
			// stack and evalute them using the current
			// operator

			vector<int> op1 = s.top();
			s.pop();

			vector<int> op2;
			if (ch[1] == (int) '+' || ch[1] == (int) '*') {
				op2 = vector<int>();
				op2.push_back(-1);
				op2.push_back(-1);
			} else {
				op2 = s.top();
				s.pop();
			}

			val = eval(op1[0], op1[1], op2[0], op2[1], ch);
			// push the value obtained after evaluating
			// onto the stack
			s.push(val);

		}
		stateStack.pop();
	}

	return val;
}

vector<int> Variables::OR(int op1, int op2, int op3, int op4) {

	cout << "oooor " << op1 << op2 << op3 << op4 << endl;

	std::vector<int> startEnd;

	int stateNum = SM.size();
	startEnd.push_back(stateNum);
	startEnd.push_back(stateNum + 1);

//create start state
	std::vector<std::vector<int>> s;
	s.push_back(createVector(-1, op1));
	s.push_back(createVector(-1, op3));

	SM.push_back(s);

//create end state
	std::vector<std::vector<int>> e;
	SM[op2].push_back(createVector(-1, stateNum + 1));
	SM[op4].push_back(createVector(-1, stateNum + 1));

	SM.push_back(e);

	return startEnd;
}

vector<int> Variables::AND(int op1, int op2, int op3, int op4) {

	cout << "and" << op1 << op2 << op3 << op4 << endl;

	std::vector<int> startEnd;

	int stateNum = SM.size();
	startEnd.push_back(stateNum);
	startEnd.push_back(stateNum + 1);

	//create start state
	std::vector<std::vector<int>> s;
	s.push_back(createVector(-1, op3));

	SM.push_back(s);

//create end state
	std::vector<std::vector<int>> e;
	SM[op2].push_back(createVector(-1, stateNum + 1));

	SM.push_back(e);

//link
	SM[op4].push_back(createVector(-1, op1));

	return startEnd;
}

vector<int> Variables::DASH(int op1, int op2, int op3, int op4) {

	int startSym = SM[op3][0][0];
	int endSym = SM[op1][0][0];

	cout << "Dash" << op1 << op2 << op3 << op4 << startSym << endl;

	std::vector<int> startEnd;

	int stateNum = SM.size();
	startEnd.push_back(stateNum);
	startEnd.push_back(stateNum + 1);

	//create start state
	std::vector<std::vector<int>> s;
	for (int i = startSym; i <= endSym; i++) {
		s.push_back(createVector(i, stateNum + 1));
	}
	SM.push_back(s);

//create end state
	SM.push_back(std::vector<std::vector<int>>());

	return startEnd;
}

vector<int> Variables::Closure(int op1, int op2, int op3, int op4) {

	cout << "closure" << op1 << op2 << op3 << op4 << endl;

	std::vector<int> startEnd;

	int stateNum = SM.size();
	startEnd.push_back(stateNum);
	startEnd.push_back(stateNum + 1);

//create start state
	std::vector<std::vector<int>> s;
	s.push_back(createVector(-1, op1));
	s.push_back(createVector(-1, stateNum + 1));

	SM.push_back(s);

//create end state
	std::vector<std::vector<int>> e;
	e.push_back(createVector(-1, stateNum));
	SM.push_back(e);

	SM[op2].push_back(createVector(-1, stateNum + 1));

	return startEnd;

}

vector<int> Variables::PClosure(int op1, int op2, int op3, int op4) {

	cout << "pclosure" << op1 << op2 << op3 << op4 << endl;

	std::vector<int> startEnd;

	int stateNum = SM.size();
	startEnd.push_back(stateNum);
	startEnd.push_back(stateNum + 1);

//create start state
	std::vector<std::vector<int>> s;
	s.push_back(createVector(-1, op1));

	SM.push_back(s);

//create end state
	std::vector<std::vector<int>> e;
	e.push_back(createVector(-1, stateNum));
	SM.push_back(e);

	SM[op2].push_back(createVector(-1, stateNum + 1));

	return startEnd;

}
