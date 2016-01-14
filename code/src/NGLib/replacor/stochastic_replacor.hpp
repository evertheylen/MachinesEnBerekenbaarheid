
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
class StochasticReplacor: public Replacor<StochasticRule<std::string>> {
public:
	using Rule_Type = StochasticRule<std::string>;
	
	StochasticReplacor(){};
	
	StochasticReplacor(TiXmlElement* elem, std::random_device::result_type seed): 
			Replacor(elem->FirstChildElement()), mt(seed) {}
	
	typename Rule_Type::NumT replace(std::string var, std::list<typename Rule_Type::NumT>& context) {
		std::uniform_int_distribution<int> dist(0, 99);
		int picked_rule = dist(mt);
		double prev_chance = 0;
		for (auto& it: cfg.rules(var)) {
			double chance = cfg.get_rule(it.second).get_chance();
			if (picked_rule - prev_chance < chance*100) {
				return it.second;
			} else {
				prev_chance += chance*100;
			}
		}
	}
	
	bool replaceable(std::string symb) {
		return cfg.has_rules(symb);
	}
	
	TiXmlElement* to_xml() {
		TiXmlElement* elem = new TiXmlElement("STOCHASTIC_REPLACOR");
		TiXmlElement* cfg_elem = cfg.to_xml();
		elem->LinkEndChild(cfg_elem);
		return elem;
	}
	
private:
	std::mt19937 mt;
};

