#include <fstream>
#include<iostream>

#include "ParserGenerator/header/Parser.h"

using namespace std;

string tokenFile = "testTokens2";
string grammerFile = "testGrammar";

int main() {

	Parser *parser = new Parser(tokenFile, grammerFile);

	parser->parse();

	return 0;
}
