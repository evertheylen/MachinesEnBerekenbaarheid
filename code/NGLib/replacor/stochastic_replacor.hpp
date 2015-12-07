
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
#include "../../libs/tinyxml/tinyxml.h"

// Stochastic replacor
class StochasticReplacor: public Replacor<StochasticRule<std::string>> {
public:
	using Rule_Type = StochasticRule<std::string>;
	
	StochasticReplacor(){};
	
	StochasticReplacor(TiXmlElement* elem, std::random_device::result_type seed): 
			mt(seed) {
		cfg = std::unique_ptr<xml_CFG<Rule_Type>>(new xml_CFG<Rule_Type>(elem->FirstChildElement()));
	}
	
	Rule_Type* replace(std::string var, std::list<StochasticRule<std::string>*>& context) {
		std::uniform_int_distribution<int> dist(0, 99);
		int picked_rule = dist(mt);
		double prev_chance = 0;
		for (auto& it: cfg->rules(var)) {
			double chance = it.second.get_chance();
			if (picked_rule - prev_chance < chance*100) {
				return ((Rule_Type*) &(it.second));
			} else {
				prev_chance += chance*100;
			}
		}
	}
	
	bool replaceable(std::string symb) {
		return cfg->has_rules(symb);
	}
	
	TiXmlElement* to_xml() {
		TiXmlElement* elem = new TiXmlElement("STOCHASTIC_REPLACOR");
		TiXmlElement* cfg_elem = cfg->to_xml();
		elem->LinkEndChild(cfg_elem);
		return elem;
	}
	
private:
	std::mt19937 mt;
	std::unique_ptr<xml_CFG<Rule_Type>> cfg;
};

