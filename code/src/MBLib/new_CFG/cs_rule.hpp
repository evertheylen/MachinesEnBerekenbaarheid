
#pragma once

#include <set>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <sstream>

#include <cassert>

#include "libs/tinyxml/tinyxml.h"
#include "MBLib/common/common.hpp"
#include "MBLib/new_CFG/CFG.hpp"


class ContextRule: public SimpleRule {
public:
	
	ContextRule() = default;
	
	ContextRule(TiXmlElement* root);
	
	void fill_table(unsigned int i);
	
	ContextRule(const std::string& _head, const std::vector<std::string>& _body, unsigned int _num);
	
	TiXmlElement* to_xml();
	
	std::map<unsigned int, double> table;
};

