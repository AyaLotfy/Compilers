/*
 * Parser.cpp
 *
 *  Created on: ??�/??�/????
 *      Author: SPIDER
 */

#include "header/Parser.h"
#include "../header/StringManp.h"
#include "header/LL1.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <string.h>

#define startSymbol 0

#define synchError -2
#define transError -3

using namespace std;

Parser::Parser(string tokensFile, string grammerFile) {
	this->tokensFile = tokensFile;
	this->grammerFile = grammerFile;
	TCounter = 0;
	NTCounter = 0;
	idx = 0;
	dollarSignIndex = 0;
	successParsing = true;
}

Parser::~Parser() {
	// TODO Auto-generated destructor stub
}

void Parser::parse() {

	createProductionGrammer();

	printGrammer();

	if (detectLeftRecusion()) {
		cout << "looooop" << endl;
		exit(1);
	}

	createFirstTable();

	createFollowTable();
	printFollowTable();
	createParsingTable();

	matchTokens();

}

void Parser::createProductionGrammer() {

	cout << "create prouduction grammer" << endl;

	ifstream infile(grammerFile);

	string line, production;

	while (std::getline(infile, line)) {

		line = trim(line);

		if (line[0] == '#') {
			if (!production.empty()) {
				processProduction(production);
			}
			production = line;
		} else {
			production.append(line);
		}
	}

	if (!production.empty()) {
		processProduction(production);
	}

}

void Parser::processProduction(string p) {
	cout << "process prodution : " << p << endl;

	//get non terminal name
	string name;
	for (idx = 1; idx < p.size() && p[idx] != '='; idx++) {
		name.push_back(p[idx]);
	}
	idx++;

	name = trim(name);

	int index = findNT(name);

	cout << name << index << endl;

	bool lambda = false;
	//evaluating production
	for (; idx < p.size(); idx++) {

		if (p[idx] == ' ' || p[idx] == '|') {
			continue;
		}
		lambda = false;

		Production prod;
		for (; idx < p.size() && p[idx] != '|';) {

			if (p[idx] == ' ') {
				idx++;
				continue;
			}
			if (p[idx] == '\\') {
				if (p[idx + 1] == 'L') {
					toLambda[index] = true;
					lambda = true;
					idx++;
					break;
				}
			}
			if (p[idx] == '\'') {
				idx++;
				prod.add(addTerminal(p), true);
			} else {
				prod.add(addNTerminal(p), false);
			}
		}
		if (!lambda) {
			grammar[index].push_back(prod);
			cout << "push production to grammer " << index << endl;
		}
	}

}

int Parser::findNT(string name) {
	//find or add to non terminals
	int index;
	auto search = NTerminalNames1.find(name);
	if (NTerminalNames1.find(name) != NTerminalNames1.end()) {
		std::cout << "Found " << search->first << " " << search->second << endl;
		index = search->second;
	} else {
		index = NTCounter;
		//std::cout << "Not foundddd : " << name << index << endl;
		NTerminalNames1.insert(pair<string, int>(name, NTCounter));
		NTerminalNames2.insert(pair<int, string>(NTCounter++, name));
		grammar.push_back(vector<Production>());
		toLambda.push_back(false);
	}
	return index;
}

int Parser::findT(string name) {
	//find or add to terminals
	int index;
	auto search = TerminalNames1.find(name);
	if (TerminalNames1.find(name) != TerminalNames1.end()) {
		std::cout << "Found " << search->first << " " << search->second << endl;
		index = search->second;
	} else {
		index = TCounter;
		//std::cout << "Not found : " << name << index << endl;
		TerminalNames1.insert(pair<string, int>(name, TCounter));
		TerminalNames2.insert(pair<int, string>(TCounter++, name));
	}
	return index;
}

int Parser::addTerminal(string p) {
	string s;
	for (; idx < p.size() && p[idx] != '\''; idx++) {
		s.push_back(p[idx]);
	}
	idx++;
	return findT(s);
}

int Parser::addNTerminal(string p) {
	string s;
	for (; idx < p.size() && p[idx] != ' ' && p[idx] != '|'; idx++) {
		s.push_back(p[idx]);
	}
	return findNT(s);
}

void Parser::printGrammer() {
	cout << "\n\nGrammer\n";
	for (unsigned i = 0; i < grammar.size(); i++) {
		cout << i << " " << NTerminalNames2.find(i)->second << " ";
		for (unsigned j = 0; j < grammar[i].size(); j++) {
			cout << "( ";
			grammar[i][j].print();
			cout << ") ";
		}
		cout << endl;
	}
}

void Parser::printProdFirst() {
	cout << "\n\nproduction first\n";
	for (unsigned i = 0; i < prodFirst.size(); i++) {
		cout << i << " " << NTerminalNames2.find(i)->second << " ";
		for (unsigned j = 0; j < prodFirst[i].size(); j++) {
			cout << "( ";
			for (unsigned k = 0; k < prodFirst[i][j].size(); k++) {
				cout << prodFirst[i][j][k] << " ";
			}
			cout << " ) " << prodFirstLambda[i][j];
		}
		cout << endl;
	}
}

void Parser::printFirstTable() {
	cout << "\n\nFirst table\n";
	for (unsigned i = 0; i < first.size(); i++) {
		cout << i << " " << NTerminalNames2.find(i)->second << " ";
		for (unsigned j = 0; j < first[i].size(); j++) {
			cout << first[i][j] << " ";
		}
		cout << " \tfirst lambda" << firstLambda[i] << endl;
	}
}

void Parser::createFirstTable() {
	InitProdFirst();
	InitFirst();
	for (unsigned i = 0; i < grammar.size(); i++) {
		getFirst(i);
	}
	printFirstTable();
	printProdFirst();
}

void Parser::InitProdFirst() {
	for (unsigned i = 0; i < grammar.size(); i++) {
		prodFirst.push_back(vector<vector<int>>());
		prodFirstLambda.push_back(vector<bool>());
	}

	for (unsigned i = 0; i < grammar.size(); i++) {
		firstLambda.push_back(false);
	}
}

void Parser::InitFirst() {
	for (unsigned i = 0; i < grammar.size(); i++) {
		first.push_back(vector<int>());
	}
}
void Parser::InitFollow() {
	for (unsigned i = 0; i < grammar.size(); i++) {
		follow.push_back(vector<int>());
	}
}

vector<int> Parser::getFirst(int index) {
	if (!first[index].empty()) {
		return first[index];
	}
	firstLambda[index] = toLambda[index];
	vector<int> s;

	//get first of each production
	for (unsigned j = 0; j < grammar[index].size(); j++) {

		vector<Symbol> symbols = grammar[index][j].PSybmols;
		vector<int> pFirst;
		unsigned k;

		//get first of each symbol in the production
		for (k = 0; k < symbols.size(); k++) {

			if (symbols[0].isTerminal) {
				pFirst.push_back(symbols[k].sybmolIndex);
				break;
			} else {
				int next = symbols[k].sybmolIndex;
				vector<int> f = getFirst(next);
				pFirst.insert(pFirst.end(), f.begin(), f.end());
				if (!firstLambda[symbols[k].sybmolIndex]) {
					break;
				}
			}
		}

		prodFirst[index].push_back(pFirst);
		prodFirstLambda[index].push_back(false);

		if (k > 0 && k == symbols.size()
				&& firstLambda[symbols[k - 1].sybmolIndex]) {
			firstLambda[index] = true;
			prodFirstLambda[index][j] = true;
		}

		s.insert(s.end(), pFirst.begin(), pFirst.end());

	}
	first[index] = s;
	return s;
}

bool Parser::detectLeftRecusion() {

	LL1 *l = new LL1(grammar, toLambda);
	return l->detectLeftRecursion();
}

vector<int> Parser::getFollow(int NT) {
	vector<int> s;
	s = follow[NT];

	for (unsigned index = 0; index < grammar.size(); index++) {
		for (unsigned i = 0; i < grammar[index].size(); i++) {
			Production prod = grammar[index][i];
			for (int j = 0; j < prod.PSybmols.size(); j++) {
				if (prod.PSybmols[j].isTerminal) {
					continue;
				}


				if (prod.PSybmols[j].sybmolIndex == NT) {

					while (j < prod.PSybmols.size() - 1) {

						if (prod.PSybmols[j + 1].isTerminal) {


							if (find(s.begin(), s.end(),
									prod.PSybmols[j + 1].sybmolIndex)
									== s.end()) {
								s.push_back(prod.PSybmols[j + 1].sybmolIndex);
							}
							break;
						}
//
						vector<int> first_of_next = getFirst(
								prod.PSybmols[j + 1].sybmolIndex);

												//							//check it reverse !=

						for (int jj = 0; jj < first_of_next.size(); jj++) {
							if (find(s.begin(), s.end(), first_of_next[jj])
									== s.end()) {
								s.push_back(first_of_next[jj]);
							}
						}



						//
						if (!firstLambda[prod.PSybmols[j + 1].sybmolIndex]) {

							break;
						}
						j++;
						//

						//
					}
					if (j == prod.PSybmols.size() - 1) {
						if (NT != index) {
							follow_part_two[NT].insert(index);


						}

					}

				}
			}
		}
	}
	return s;
}

vector<int> Parser::getFollow2(int NT) {
	if (visited.find(NT) != visited.end()) {
		return getFollowByIdx(NT);
	}

	visited.insert(NT);


	for (auto entry : follow_part_two) {
		unordered_set<int> value = entry.second;
		for (auto strIdx : value) {
			follow[strIdx] = getFollow2(strIdx);
			vector<int> f1 = follow[entry.first];
			vector<int> f2 = follow[strIdx];

			for (int jj = 0; jj < f2.size(); jj++) {
				if (find(f1.begin(), f1.end(), f2[jj]) == f1.end()) {
					f1.push_back(f2[jj]);
				}
			}
			follow[entry.first] = f1;
		}
	}
	return follow[NT];

}

void Parser::createParsingTable() {
	build1();
	build2();
}

//for each production rule A = B of a grammar G
//for each terminal a in FIRST(B)
//add A = B to M[A,a]
void Parser::build1() {
	for (unsigned index = 0; index < grammar.size(); index++) {
		vector<map<int, Production>> vecParsing;
		for (unsigned i = 0; i < grammar[index].size(); i++) {
			map<int, Production> mapParsing;
			parsingTable.push_back(mapParsing);
		}
	}


	for (unsigned i = 0; i < prodFirst.size(); i++) {

		map<int, Production> mapProd = parsingTable[i];
		for (unsigned j = 0; j < prodFirst[i].size(); j++) {

			for (unsigned k = 0; k < prodFirst[i][j].size(); k++) {
				if (mapProd.find(prodFirst[i][j][k]) == mapProd.end()) {

					mapProd[prodFirst[i][j][k]] = grammar[i][j];
				} else {

					if (!isEqualProdctions(mapProd[prodFirst[i][j][k]],
							grammar[i][j])) {
						reportAmbiguity();
					}


				}
			}
		}

		parsingTable[i] = (mapProd);


	}
	printParsingTable();

}
void Parser::printParsingTable() {
	cout << "printParsingTable\n";
	for (int i = 0; i < parsingTable.size(); ++i) {
		//  cout<< "" <<NTerminalNames2.find(i)->second <<"\n";
		map<int, Production> mapParsing = parsingTable[i];
		for (auto& x : mapParsing) {

			cout << NTerminalNames2.find(i)->second << " :";
			//	cout << i <<" :";

			//cout << NTerminalNames2.find(i)->second <<" : " ;
			std::cout << TerminalNames2.find(x.first)->second << ": ";
			for (int j = 0; j < x.second.PSybmols.size(); ++j) {
				if (x.second.PSybmols[j].isTerminal) {
					cout
							<< TerminalNames2.find(
									x.second.PSybmols[j].sybmolIndex)->second
							<< "";
				} else {
					cout
							<< NTerminalNames2.find(
									x.second.PSybmols[j].sybmolIndex)->second
							<< "";

				}
				cout << " ";
			}
			cout << "\n";

		}

	}
}
vector<int> Parser::getFollowByIdx(int idx) {
	return follow[idx];
}
//for each production rule A = B of a grammar G
//If \L  in FIRST(B)
 //for each terminal a in FOLLOW(A) add A = B to M[A,a]
void Parser::build2() {
	for (unsigned index = 0; index < grammar.size(); index++) {
		map<int, Production> mapProd = parsingTable[index];
		if (toLambda[index] == true) {
			for (int var = 0; var < getFollowByIdx(index).size(); ++var) {
				Production prod;
				prod.add(findT("\\L"), true);

				if (mapProd.find(getFollow(index)[var]) == mapProd.end()) {


					mapProd[getFollowByIdx(index)[var]] = prod;

				} else {
					if (!isEqualProdctions(mapProd[getFollowByIdx(index)[var]],
							prod)) {
						reportAmbiguity();
					}


				}

			}
		}
		parsingTable[index] = mapProd;
	}
	printParsingTable();
}
//
void Parser::build3() {
	for (unsigned index = 0; index < grammar.size(); index++) {
		for (int i = 0; i < grammar[i].size(); ++i) {

			map<int, Production> mapProd = parsingTable[index];
			if (prodFirstLambda[index][i] == true) {
				for (int var = 0; var < getFollow(index).size(); ++var) {
					if (getFollow(index)[var] == NTerminalNames1["$"]) {
						Production prod;
						prod.add(findT("\\L"), true);

						if (mapProd.find(NTerminalNames1["$"])
								== mapProd.end()) {


							mapProd[getFollowByIdx(index)[var]] = prod;

						} else {
							if (!isEqualProdctions(
									mapProd[getFollowByIdx(index)[var]],
									prod)) {
								reportAmbiguity();
							}



						}
					}
				}
			}
			parsingTable[index] = mapProd;
		}

	}

}

//

bool Parser::isEqualProdctions(Production x, Production p) {

	if (x.PSybmols.size() != p.PSybmols.size()) {
		return false;
	}
	for (int j = 0; j < x.PSybmols.size(); ++j) {
		if (x.PSybmols[j].isTerminal) {
			if (!p.PSybmols[j].isTerminal) {
				return false;
			}
			string s = TerminalNames2.find(x.PSybmols[j].sybmolIndex)->second;

			string ss = TerminalNames2.find(p.PSybmols[j].sybmolIndex)->second;

			if (s != ss) {

				return false;
			}

		} else {
			if (p.PSybmols[j].isTerminal) {
				return false;
			}
			string s = NTerminalNames2.find(x.PSybmols[j].sybmolIndex)->second;

			string ss = NTerminalNames2.find(p.PSybmols[j].sybmolIndex)->second;

			if (s != ss) {
				return false;
			}


		}
	}

	return true;
}
void Parser::reportAmbiguity() {
	cout << "Error: Ambiguous Grammer";
	exit(0);
}

void Parser::createFollowTable() {

	InitFollow();

	dollarSignIndex = findT("$");


	vector<int> s;


	s.push_back(dollarSignIndex);

	string str = TerminalNames2.find(dollarSignIndex)->second;


	follow[0] = s;



	for (unsigned i = 0; i < follow.size(); i++) {
		follow[i] = getFollow(i);

	}

	printFollowTable();

	print_follow_part_two();
	for (auto entry : follow_part_two) {

		getFollow2(entry.first);

	}
	printFollowTable();

}

void Parser::printFollowTable() {
	cout << " print follow table\n";

	for (int i = 0; i < follow.size(); ++i) {
		for (int j = 0; j < follow[i].size(); ++j) {
			cout << NTerminalNames2.find(i)->second << " "
					<< TerminalNames2.find(follow[i][j])->second << "\n";

		}
	}

}
void Parser::print_follow_part_two() {
	for (auto entry : follow_part_two) {


		cout << NTerminalNames2.find(entry.first)->second << " contains ";
		unordered_set<int> temp = entry.second;
		for (auto f : temp) {
			cout << NTerminalNames2.find(f)->second << " ";
		}
		cout << "\n";
	}
}

void Parser::matchTokens() {

	getTokens(); //get tokens from file to tokens array
	InitMatchingStack(); //initialize stack for parsing

	cout << "left most derivation tree" << endl;
	printLeftmostDrivation(0);

	int x = match(0);

	//excess input
	printExcessInput(x);

	//missing
	printMissingSymbols();

	if (successParsing) {
		cout << "Success parsing" << endl;
	} else {
		cout << "parsing with errors" << endl;
	}
}

void Parser::printExcessInput(int index) {
	for (; index < tokens.size(); index++) {
		successParsing = false;
		cout << "excess " << tokens[index] << endl;
	}
}

void Parser::printMissingSymbols() {
	while (!matchingStack.empty()) {
		successParsing = false;
		Symbol top = matchingStack.back();
		if (top.isTerminal) {
			cout << "missing " << getTerminalName(top.sybmolIndex) << endl;
		} else {
			cout << "missing " << getNTerminalName(top.sybmolIndex) << endl;
		}
		matchingStack.pop_back();
	}
}

void Parser::getTokens() {
	std::ifstream infile(tokensFile);

	std::string line;
	while (std::getline(infile, line)) {
		//std::cout << "read line " << line << endl;
		tokens.push_back(trim(line));
	}
}

void Parser::InitMatchingStack() {
	matchingStack.push_back(Symbol(dollarSignIndex, true)); //initializing stack
	matchingStack.push_back(Symbol(startSymbol, false)); //initializing stack
}

int Parser::getTerminalIndex(string token) {
	auto search = TerminalNames1.find(token);
	if (TerminalNames1.find(token) != TerminalNames1.end()) {
		//std::cout << "Found " << search->first << " " << search->second << endl;
		return search->second;
	} else {
		return -1;
	}
}
string Parser::getTerminalName(int index) {
	auto search = TerminalNames2.find(index);
	if (TerminalNames2.find(index) != TerminalNames2.end()) {
		//std::cout << "Found " << search->first << " " << search->second << endl;
		return search->second;
	} else {
		return "";
	}
}
string Parser::getNTerminalName(int index) {
	auto search = NTerminalNames2.find(index);
	if (NTerminalNames2.find(index) != NTerminalNames2.end()) {
		//std::cout << "Found " << search->first << " " << search->second << endl;
		return search->second;
	} else {
		return "";
	}
}

void Parser::printProduction(Symbol NT, Production prod) {

	cout << "\t\t\t\t\t" << getNTerminalName(NT.sybmolIndex) << " ==> ";
	for (int i = 0; i < prod.PSybmols.size(); i++) {
		Symbol s = prod.PSybmols[i];
		if (s.isTerminal) {
			cout << getTerminalName(s.sybmolIndex) << " ";
		} else {
			cout << getNTerminalName(s.sybmolIndex) << " ";
		}
	}
	cout << endl;
}

int Parser::replaceWithProduction(Symbol NT, int input) {

	map<int, Production> mp = parsingTable[NT.sybmolIndex];
	Production prod;
	auto search = mp.find(input);
	if (mp.find(input) != mp.end()) {
		prod = search->second;
	} else {
		//error not found in parsing table
		return NoTransisionError(NT, input);
	}
	matchingStack.pop_back();
	printProduction(NT, prod);
	for (int i = prod.PSybmols.size() - 1; i >= 0; i--) {
		Symbol s = prod.PSybmols[i];
		if (s.isTerminal) {
			string t = getTerminalName(s.sybmolIndex);
			if (t.compare("\\L") == 0) {
				break;
			}
		}
		matchingStack.push_back(prod.PSybmols[i]);
	}
	return 0;
}
int Parser::NoTransisionError(Symbol NT, int input) {
	//search in follow
	for (unsigned i = 0; i < follow[NT.sybmolIndex].size(); i++) {
		if (input == follow[NT.sybmolIndex][i]) {
			return synchError; // found in follow synchronization
		}
	}
	return transError; //error
}

int Parser::match(int index) {

	if (index >= tokens.size() | matchingStack.empty()) {
		return index;
	}

	//cout << "\tsearch for token " << tokens[index] << endl;

	int input = getTerminalIndex(tokens[index]);
	if (input == -1) {
		//error terminal not found
		//cout << "error: terminal  " << tokens[index] << "not found" << endl;
		successParsing = false;
		return match(index + 1);
	}

	Symbol stTop = matchingStack[matchingStack.size() - 1]; //stack top

	if (stTop.isTerminal) {
		string t = getTerminalName(stTop.sybmolIndex);

		if (t.compare(tokens[index]) == 0) {
			cout << "\tmatch token " << t << endl;
			matchingStack.pop_back();
			return match(index + 1);
		} else {
			////error two terminals
			successParsing = false;
			cout << "\tmissing " << t << endl;
			matchingStack.pop_back();
			printLeftmostDrivation(index);
			return match(index);
		}
	} else {
		int x = replaceWithProduction(stTop, input);
		if (x == 0) {
			printLeftmostDrivation(index);
			return match(index);
		} else if (x == synchError) {
			//synchronization
			successParsing = false;
			cout << "\tmissing " << getNTerminalName(stTop.sybmolIndex) << endl;
			matchingStack.pop_back();
			printLeftmostDrivation(index);
			return match(index);
		} else if (x == transError) {
			//error
			successParsing = false;
			cout << "\texcess " << tokens[index] << endl;
			tokens[index] = ""; //remove it from input
			return match(index + 1);
		}
	}
	return index;
}

void Parser::printLeftmostDrivation(int index) {
	//print input
	for (int i = 0; i < index; i++) {
		cout << tokens[i] << " ";
	}

	//print stack
	for (int i = matchingStack.size() - 1; i >= 0; i--) {

		if (matchingStack[i].isTerminal) {
			cout << getTerminalName(matchingStack[i].sybmolIndex) << " ";
		} else {
			cout << getNTerminalName(matchingStack[i].sybmolIndex) << " ";
		}
	}

	cout << endl;
}
