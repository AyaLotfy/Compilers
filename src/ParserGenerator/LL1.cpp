/*
 * LL1.cpp
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#include "header/LL1.h"

using namespace std;

LL1::LL1(vector<vector<Production>> grammer, vector<bool> toLambda) {

	this->grammer = grammer;
	this->toLambda = toLambda;
}

LL1::~LL1() {
	// TODO Auto-generated destructor stub
}

bool LL1::detectLeftRecursion() {

	int n = grammer.size();
	for (int i = 0; i < n; i++) {
		if (DFS(i, InitVisited(n))) {
			return true;
		}
	}
	return false;
}

bool LL1::DFS(int x, vector<bool> visited) {
	if (visited[x]) {
		return true;
	}
	visited[x] = true;
	for (unsigned i = 0; i < grammer[x].size(); i++) {
		vector<Symbol> s = grammer[x][i].PSybmols;
		for (unsigned j = 0; j < s.size(); j++) {
			if (s[j].isTerminal) {
				break;
			} else {
				int next = s[j].sybmolIndex;
				if (DFS(next, copy(visited))) {
					return true;
				} else {
					if (!toLambda[next]) {
						break;
					}
				}
			}
		}
	}
	return false;
}
vector<bool> LL1::InitVisited(int n) {
	vector<bool> visited;
	for (int i = 0; i < n; i++) {
		visited.push_back(false);
	}
	return visited;
}

vector<bool> LL1::copy(vector<bool> v1) {
	vector<bool> v2;
	for (unsigned i = 0; i < v1.size(); i++) {
		v2.push_back(v1[i]);
	}
	return v2;
}
