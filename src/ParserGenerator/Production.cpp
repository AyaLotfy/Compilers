/*
 * Production.cpp
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#include "../ParserGenerator/header/Production.h"
#include <iostream>

Production::Production() {
	// TODO Auto-generated constructor stub

}

Production::~Production() {
	// TODO Auto-generated destructor stub
}

void Production::add(int index, bool isTerminal) {
	PSybmols.push_back(Symbol(index, isTerminal));
}

void Production::print() {
	for (unsigned i = 0; i < PSybmols.size(); i++) {
		if(PSybmols[i].isTerminal) {
			std::cout << PSybmols[i].sybmolIndex << "T " ;
		} else {
			std::cout << PSybmols[i].sybmolIndex << "NT " ;
		}
	}
}
