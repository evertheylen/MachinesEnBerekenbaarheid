
#pragma once

#include <string>
#include <vector>
#include <list>
#include <random>
#include <memory>
#include <exception>

#include "MBLib/new_CFG/CFG.hpp"
#include "MBLib/new_CFG/s_rule.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "tinyxml/tinyxml.h"

// Stochastic replacor
class StochasticReplacor: public CfgReplacor<StochasticRule> {
public:
// 	using RuleT = StochasticRule;
	
	StochasticReplacor(){};
	
	StochasticReplacor(TiXmlElement* elem, std::random_device::result_type seed): 
			CfgReplacor(elem->FirstChildElement()), mt(seed) {} // TODO seed
	
	unsigned int replace(std::string var, std::list<unsigned int>& context);
	
	bool replaceable(std::string symb);
	
	TiXmlElement* to_xml();
	
private:
	std::mt19937 mt;
};

