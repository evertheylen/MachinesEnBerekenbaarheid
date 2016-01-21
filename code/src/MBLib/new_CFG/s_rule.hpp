
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

class StochasticRule: public SimpleRule {
public:
	
	StochasticRule() = default;
	
	StochasticRule(TiXmlElement* root);
	
	StochasticRule(const std::string& _head, const std::vector<std::string>& _body, unsigned int _num, double _chance);
	
	TiXmlElement* to_xml();
	
	double get_chance() const;
	
	double chance;
};

