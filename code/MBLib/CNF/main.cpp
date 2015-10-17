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
	doc_cfg.LoadFile("CNF/CFGS/CFG.xml");
	CFG cfg_1(doc_cfg);
	eliminate_epsilon_productions(cfg_1);
}


