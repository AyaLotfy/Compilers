/*
 * Symbol.cpp
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#include "../ParserGenerator/header/Symbol.h"

Symbol::Symbol(int index, bool isTerminal) {
	this->isTerminal = isTerminal;
	this->sybmolIndex = index;
}

Symbol::~Symbol() {
	// TODO Auto-generated destructor stub
}

