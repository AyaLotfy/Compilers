/*
 * Symbol.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#ifndef PARER_SYMBOL_H_
#define PARER_SYMBOL_H_

class Symbol {
public:
	Symbol(int, bool);
	virtual ~Symbol();

	int sybmolIndex;
	bool isTerminal;
};

#endif /* PARER_SYMBOL_H_ */
