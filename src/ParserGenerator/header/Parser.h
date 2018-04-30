/*
 * Parser.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#ifndef PARSERGENERATOR_HEADER_PARSER_H_
#define PARSERGENERATOR_HEADER_PARSER_H_

#include <fstream>
#include <map>
#include <stack>

#include <unordered_map>
#include <unordered_set>

#include "../../ParserGenerator/header/Production.h"
#include "LL1.h"

using namespace std;

class Parser {
public:
	Parser(string tokensFile, string grammerFile);
	virtual ~Parser();

	void parse();
private:

	void createProductionGrammer();
	bool detectLeftRecusion();
	void createFirstTable();
	void createFollowTable();
	void createParsingTable();
	void matchTokens();

	//used in parsing grammar
	void processProduction(string);
	int getNTname(string);
	int findNT(string);
	int findT(string);
	int addTerminal(string);
	int addNTerminal(string);

	//used in calculating first
	vector<int> getFirst(int);
	void InitProdFirst();
	void InitFirst();

	//used in calculating follow
	vector<int> getFollow(int);
	vector<int> getFollow2(int);

	void InitFollow();

	void build1();
	void build2();
	void build3();

	vector<int> getFollowByIdx(int);

	void reportAmbiguity();
	bool isEqualProdctions(Production p1, Production p2);



	//used in matching tokens
	void getTokens(); //get tokens from file to vector tokens
	void InitMatchingStack(); //initialize stack
	int match(int); //recursive main matching function
	int getTerminalIndex(string);
	string getTerminalName(int);
	string getNTerminalName(int);
	int replaceWithProduction(Symbol, int);
	void printExcessInput(int);
	void printMissingSymbols();

	//for matching error handling
	int NoTransisionError(Symbol, int);
	void printLeftmostDrivation(int);

	//used in debugging
	void printGrammer();
	void printFirstTable();
	void printProdFirst();
	void printFollowTable();
	void print_follow_part_two();
	void printParsingTable();
	void printProduction(Symbol, Production);

	unsigned idx; //used in parsing grammer

	string tokensFile;
	string grammerFile;

	vector<vector<Production>> grammar; //hold the grammar each non terminal by its productions
	vector<bool> toLambda; //give indication if the non terminals go to \L

	vector<vector<vector<int>>> prodFirst; // first terminal of each production
	vector<vector<bool>> prodFirstLambda; //is true if the first terminal of production is \L

	//hold terminal names with its indexes
	map<string, int> TerminalNames1;
	map<int, string> TerminalNames2;
	int TCounter; //counter of terminals

	//hold non terminal names with its indexes
	map<string, int> NTerminalNames1;
	map<int, string> NTerminalNames2;
	int NTCounter; //counter of non terminals

	//hold the first terminal of each non terminal
	vector<vector<int>> first;
	vector<bool> firstLambda; // indicate if the first terminal of this non terminals is \L

	int dollarSignIndex;

	//hold the follow terminal of each non terminal
	vector<vector<int>> follow;
	unordered_map<int, unordered_set<int>> follow_part_two;
	unordered_set<int> visited;

	vector<map<int, Production>> parsingTable;

	//contain the tokens scanned from tokenFile
	vector<string> tokens;

	vector<Symbol> matchingStack;
	bool successParsing; //is true if parsing is success

};

#endif /* PARSERGENERATOR_HEADER_PARSER_H_ */
