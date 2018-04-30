/*
 * LL1.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#ifndef PARSERGENERATOR_LL1_H_
#define PARSERGENERATOR_LL1_H_

#include "Production.h"

#include<vector>
#include<iostream>

class LL1 {
public:
	LL1(std::vector<std::vector<Production>>, std::vector<bool>);
	virtual ~LL1();

	bool detectLeftRecursion();
private:
	std::vector<std::vector<Production>> grammer;
	std::vector<bool> toLambda;
	//std::vector<bool> visited;

	std::vector<bool> InitVisited(int);
	std::vector<bool> copy(std::vector<bool>);
	bool DFS(int, std::vector<bool>);
};

#endif /* PARSERGENERATOR_LL1_H_ */
