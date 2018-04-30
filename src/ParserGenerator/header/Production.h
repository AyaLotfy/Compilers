/*
 * Production.h
 *
 *  Created on: ??�/??�/????
 *      Author: SPIDER
 */


#ifndef PARER_PRODUCTION_H_
#define PARER_PRODUCTION_H_


#include <vector>

#include "../../ParserGenerator/header/Symbol.h"


class Production {
public:
	Production();
	virtual ~Production();

	void add(int, bool);
	void print();
	std::vector<Symbol> PSybmols;
};

#endif /* PARER_PRODUCTION_H_ */
