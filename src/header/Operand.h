/*
 * Operand.h
 *
 *  Created on: ??�/??�/????
 *      Author: SPIDER
 */

#ifndef OPERAND_H_
#define OPERAND_H_

#include <string>

class Operand {
public:

	int index;
	std::string op;

	Operand(int index, std::string op);
	virtual ~Operand();
};

#endif /* OPERAND_H_ */
