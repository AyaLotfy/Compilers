
#include "project.h"

//#include "DFA.h"


using namespace std;

vector<string> lexical_rules;

/*
DFA *minimized;
map<int, string> mp;

project::project(vector<string> &list) {
    lexical_rules = list;
}

void project::initialize() {
	//initialize
	//minimized



}


vector<string> project::output(vector<string> &code) {

    ofstream tokenFile;
    tokenFile.open("Token.txt");
    vector<string> matches = DFA::match_dfa(minimized, code, mp);
    for (auto x : matches) {
        tokenFile << x << endl;
    }
    tokenFile.close();

    return matches;
}


*/
