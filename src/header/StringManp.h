/*
 * StringManp.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#ifndef HEADER_STRINGMANP_H_
#define HEADER_STRINGMANP_H_

#include <string>
#include <iostream>
#include <ctype.h>

#include "Operand.h"

#define Epthnon "Epthnon"

std::string trim(std::string line);
std::string removeBraces(std::string line);
std::string substring(std::string line, int from, int to);
std::string removeSBrackets(std::string line);

std::string getFirstWord(std::string line);
int getLHSIndex(std::string line);
Operand getOperand(std::string line, int index);

#endif /* HEADER_STRINGMANP_H_ */
