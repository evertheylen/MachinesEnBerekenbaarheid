/*
 * main.cpp
 *
 *  Created on: 17 Oct 2015
 *      Author: stijn
 */

#include "CNF.hpp"
#include "../CFG.hpp"
#include "../libs/tinyxml/tinyxml.h"
#include <iostream>

int main() {
	TiXmlDocument doc_cfg;
	doc_cfg.LoadFile("CNF/CFGS/random_cfg.xml");
	CFG cfg(doc_cfg);
	CFG  cleaned_up_cfg = eliminate_useless_symbols(eliminate_unit_pairs(eliminate_epsilon_productions(cfg))); // Step one for cnf: clean up grammar.
	std::cout << cleaned_up_cfg << std::endl;
}


