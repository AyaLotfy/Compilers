/*
 * ProductionTable.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: SPIDER
 */

#ifndef PARSERGENERATOR_PRODUCTIONTABLE_H_
#define PARSERGENERATOR_PRODUCTIONTABLE_H_

#include <vector>
#include <string>

class ProductionTable {
public:
	ProductionTable();
	virtual ~ProductionTable();

	//add(int )
private:
	std::vector<std::string> nonTerminals;
	std::vector<std::vector<std::string>> production;
};

#endif /* PARSERGENERATOR_PRODUCTIONTABLE_H_ */
