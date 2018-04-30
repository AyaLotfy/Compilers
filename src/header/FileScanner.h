/*
 * FileScanner.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#ifndef FILESCANNER_H_
#define FILESCANNER_H_

#include "AcceptedState.h"
#include "StringManp.h"
#include "Variables.h"
#include "Expression.h"
#include "DFA.h"

#include <string>
#include <vector>
#include <stack>

extern std::vector<std::vector<std::vector<int>>> NFS;

extern std::vector<AcceptedState> acceptedStates;

extern std::vector<std::vector<std::vector<std::vector<int>>>> variables;

extern std::vector<std::vector<int>> variableStartEnd;

extern std::vector<std::string> variableName;

std::vector<int> addOperand(std::string operand);
void printNFS();

#endif /* FILESCANNER_H_ */
