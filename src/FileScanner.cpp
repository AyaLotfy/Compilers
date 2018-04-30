/*
 * FileScanner.cpp
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#include "header\FileScanner.h"
#include "Utilities.h"
#include <fstream>
#include <iostream>
#include <sstream>

void processLine(std::string line);

void scanKeywords(std::string line, int index);
void scanPunctuations(std::string line, int index);
void scanStatement(std::string line, int index);
void scanDefinition(std::string line, int index);
void scanExpression(std::string line, int index);

int addKeyword(std::string line, int index);
int addPunctuation(std::string line, int index);

string removeEscape(string line);
void InitializeEpthnon();

vector<string> match_dfa(vector<vector<vector<int>>> dfa,
vector<string> input);
void processLine(std::string line);

void printSM(std::vector<std::vector<std::vector<int>>> SM);

void printAcceptedStates();

std::vector<std::vector<std::vector<int>>> NFS;
std::vector<AcceptedState> acceptedStates;

std::vector<std::vector<std::vector<std::vector<int>>>> variables;

std::vector<std::string> variableName;

std::vector<std::vector<int>> variableStartEnd;

int main111111() {

	//initialization
	NFS.push_back(std::vector<std::vector<int>>());

	InitializeEpthnon();

	std::ifstream infile("thefile.txt");

	std::string line;
	while (std::getline(infile, line)) {
		std::cout << "read line" << line << endl;
		processLine(line);
	}

	printAcceptedStates();
cout<<"salmmmmmmmmmmma"<<endl;
	std::vector<std::vector<std::vector<int>>> dfa = NFAtoDFA();
    cout << "bef file cod";

vector<string> code = Utilities::read_file("code2.txt");
	cout << "after code";
	vector<string> matches = match_dfa(dfa, code);
	cout << "after match";
	//vector < string > matchVector = SMInterface::match_dfa(dfa, code);
	for (unsigned i = 0; i < matches.size(); i++) {

		cout << matches[i] << "\n";
	}

	ofstream tokenFile;
	tokenFile.open("Out.txt");
	for (auto x : matches) {
		tokenFile << x << endl;
	}
	tokenFile.close();

	return 0;
}

void processLine(std::string line) {

	unsigned i;

	//escape initil spaces
	for (i = 0; i < line.length(); i++) {
		if (line[i] != ' ')
			break;
	}

	//check first character
	switch (line[i]) {
	case '{':
		//Keywords
		scanKeywords(line, i);
		break;
	case '[':
		//Punctuations
		scanPunctuations(line, i);
		break;
	default:
		//normal statement
		scanStatement(line, i);
	}
}

void scanKeywords(std::string line, int index) {

	line = removeBraces(line);

	std::cout << "after removing spaces:" << line << endl;

	//check braces
	if (line.empty()) /////may generate error ?????
		return;

	unsigned i = 0;

	while (i < line.length()) {
		i = addKeyword(line, i);
	}

}

//add keyword to NFS
int addKeyword(std::string line, int index) {

	if (line[index] == ' ')
		return index + 1;

	int stateNum = NFS.size();

	//add to start state
	std::vector<int> v = std::vector<int>();
	v.push_back(-1);
	v.push_back(stateNum);
	NFS[0].push_back(v);

	unsigned i;
	for (i = index; i < line.length() && line[i] != ' '; i++, stateNum++) {

		std::vector<int> v = std::vector<int>();
		v.push_back(line[i]);
		v.push_back(stateNum + 1);

		std::vector<std::vector<int>> state = std::vector<std::vector<int>>();
		state.push_back(v);

		NFS.push_back(state);
	}

	//final accepted state
	std::vector<std::vector<int>> state = std::vector<std::vector<int>>();

	NFS.push_back(state);

	std::cout << "key word is detedted: " << substring(line, index, i - 1)
			<< endl;

	acceptedStates.push_back(
			AcceptedState(substring(line, index, i - 1), stateNum));

	printNFS();

	return i;
}

void printSM(std::vector<std::vector<std::vector<int>>> SM) {

	std::cout << "SM \n";

	for (unsigned i = 0; i < SM.size(); i++) {
		for (unsigned j = 0; j < SM[i].size(); j++) {
			std::cout << (char) SM[i][j][0] << " " << SM[i][j][1] << "\t";
		}
		std::cout << "\n";
	}
}

void printNFS() {

	std::cout << "NFS \n";

	for (unsigned i = 0; i < NFS.size(); i++) {
		for (unsigned j = 0; j < NFS[i].size(); j++) {
			std::cout << i << " " << (char) NFS[i][j][0] << " " << NFS[i][j][1]
					<< "\t";
		}
		std::cout << "\n";
	}
}

void printAcceptedStates() {
	for (unsigned i = 0; i < acceptedStates.size(); i++) {
		std::cout << acceptedStates[i].stateName << ","
				<< acceptedStates[i].stateNumber << "\n";
	}
}

void scanPunctuations(std::string line, int index) {

	line = removeSBrackets(line);

	std::cout << "after removing square brackets:" << line << "\n";

	//check braces
	if (line.empty()) /////may generate error ?????
		return;

	unsigned i = 0;

	while (i < line.length()) {
		i = addPunctuation(line, i);
	}

	printNFS();
	printAcceptedStates();

}

int addPunctuation(std::string line, int index) {

	if (line[index] == ' ')
		return index + 1;

	if (line[index] == '\\')
		index++;

	int stateNum = NFS.size();

	//add to start state
	std::vector<int> v = std::vector<int>();
	v.push_back(line[index]);
	v.push_back(stateNum);
	NFS[0].push_back(v);

	//final state
	std::vector<std::vector<int>> state = std::vector<std::vector<int>>();
	NFS.push_back(state);

	std::string s;
	s.push_back(line[index]);
	acceptedStates.push_back(AcceptedState(s, stateNum));

	return index + 1;
}

void scanStatement(std::string line, int index) {

	line = trim(line);
	int idx = getLHSIndex(line);

	if (idx == -1)
		return;

	switch (line[idx]) {
	case '=':
		scanDefinition(line, idx);
		break;
	case ':':
		scanExpression(line, idx);
		break;
	}
}

void scanDefinition(std::string line, int index) {

	//add to variable name
	variableName.push_back(trim(substring(line, 0, index - 1)));

	Variables v = Variables(substring(line, index + 1, line.length() - 1));
	variableStartEnd.push_back(v.evalPostfix());

	//add state machine to variable States
	variables.push_back(v.SM);

	printSM(v.SM);

}

void scanExpression(std::string line, int index) {

	Expression e = Expression(substring(line, index + 1, line.length() - 1));

	vector<int> SE = e.evalPostfix();

	vector<int> v;
	v.push_back(-1);
	v.push_back(SE[0]);
	NFS[0].push_back(v);

	AcceptedState AccState = AcceptedState(trim(substring(line, 0, index - 1)),
			SE[1]);

	acceptedStates.push_back(AccState);

	printNFS();

}
void InitializeEpthnon() {

	cout << "initialize epthnon " << endl;

	std::vector<int> startEnd;
	startEnd.push_back(0);
	startEnd.push_back(1);

	vector<vector<vector<int>>> SM;

	std::vector<int> v;
	v.push_back(-1);
	v.push_back(1);

	std::vector<std::vector<int>> s;
	s.push_back(v);

	SM.push_back(s);

	SM.push_back(vector<vector<int>>());

	variableName.push_back(Epthnon);
	variableStartEnd.push_back(startEnd);
	variables.push_back(SM);
}

int transition(std::vector<std::vector<std::vector<int>>> SM, int st,
		int input) {

	std::vector<std::vector<int>> currentState = SM[st];

	for (unsigned i = 0; i < currentState.size(); i++) {
		if (currentState[i][0] == input) {

			return currentState[i][1];
		}
	}

	//there is no transition under this input
	return -1;
}

std::string stateIsAccepted(int st) {

	for (unsigned i = 0; i < acceptedStates.size(); i++) {
		if (acceptedStates[i].stateNumber == st) {
			return acceptedStates[i].stateName;
		}
	}

	//not accepted
	return "";

}

vector<string> match_dfa(vector<vector<vector<int>>> dfa,
		vector<string> input) {
	vector<string> machedVector;
	int st = 48;
	int start = 0;
	cout << input.size();
	for (unsigned count = 0; count < input.size(); count++) {
		string in = input[count];
		cout << input[count] << endl;

		int last_match = -1;
		unsigned int last_idx = 0;
		unsigned int i = 0;
		while (i < in.size()) {
			if (in[i] == ' ' || in[i] == '\t') {
				if (last_match == -1) {
							cout <<"faraa3'";
							cout <<"acc "<< stateIsAccepted(st - 48)<< endl;
										cout << "no match aya22222222"<<endl;
					machedVector.push_back("no match possible");
					break;
				} else if (last_match == i - 1) {
									cout<< "lessaa match now";

									cout <<"accccc"<< stateIsAccepted(st - 48)<< endl;
					machedVector.push_back(stateIsAccepted(st - 48));
							cout <<stateIsAccepted(st-48)<<"sokkar";

					while (i < in.size() && (in[i] == ' ' || in[i] == '\t')) {
						i++;
					}
					last_idx = i;
					last_match = -1;

					st = 48;
					//	start =0;
				} else {
					cout <<"accc"<< stateIsAccepted(st - 48)<< endl;
								cout << "no match aya1111111\n";
					machedVector.push_back("no match possible");
					break;

				}
			} else {
				cout <<"elseeeeeeeeee"<<endl;
				 cout <<st<<"state" <<endl;
				 cout <<int(in[i])<<"input"<<endl;
				 cout <<start << " stttttttttart****" <<endl;

				 cout <<transition(dfa, st-48 , int(in[i]))<<"dist"<<endl;


				if (transition(dfa, st - 48, int(in[i])) == -1) {
					cout <<"-111111111111\n";

					if (last_match == -1) {
						cout <<"acc"<< stateIsAccepted(st - 48)<< endl;
									cout << "no match ayaaaaaaaaaaaa"<<endl;
						machedVector.push_back("no match possible");
						break;
					} else {

						cout << "3'alaaaaaat"<<endl;
						machedVector.push_back(stateIsAccepted(st - 48));
						last_idx = last_match + 1;
						i = last_idx;
						last_match = -1;
						st = 48;

					}
				} else {
					cout << "999999999999999"<<endl;

					cout << st << "state2" <<endl ;
					cout << int(in[i]) << "input2"<<endl;
					cout <<start << " stttttttttart"<<endl;
					cout << transition(dfa, st - 48, int(in[i])) << "dist2"<<endl;
					st = transition(dfa, st - 48, int(in[i]));
					st += 48;
					//st = st-48;
					cout <<int(st)<<"\n";
					string check = stateIsAccepted(st - 48);
					cout << check << "cheeeeeeeeeeeeck"<<endl;

					if (check == "") {

					} else {
						last_match = i;
						cout << "aya baskota<3"<<endl;
					}
				}
				i++;
			}
		}
		if (i == in.size()) {
			if (last_match == -1 && last_idx < in.size()) {
				cout <<"wrooooooooong"<<endl;
				cout <<"acc"<< stateIsAccepted(st - 48)<< endl;
				cout << "no match aya"<<endl;
				machedVector.push_back("no match possible");
				break;
			} else {
					cout <<"finaaaaaaaaaaaaaaaaaal"<<endl;

				machedVector.push_back(stateIsAccepted(st - 48));

				last_idx = last_match + 1;
				i = last_idx;
				last_match = -1;

				st = 48;
			}
		}

	}

	return machedVector;

}

