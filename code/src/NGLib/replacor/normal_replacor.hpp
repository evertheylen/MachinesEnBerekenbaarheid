
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
class NormalReplacor: public Replacor<SimpleRule<std::string>> {
public:
	using Rule_Type = SimpleRule<std::string>;
	
	NormalReplacor() {};
	
	NormalReplacor(TiXmlElement* elem, std::random_device::result_type seed):
			Replacor(elem->FirstChildElement()), mt(seed) {}
	
	typename Rule_Type::NumT replace(std::string var, std::list<typename Rule_Type::NumT>& context) {
		std::uniform_int_distribution<int> dist(0, 99);
		int picked_rule = dist(mt);
		double prev_chance = 0;
		int total_possibilities = cfg.rules(var).size();
		for (auto& it: cfg.rules(var)) {
			double chance = prev_chance + (100/total_possibilities);
			if (picked_rule - prev_chance < chance) {
				return it.second;
			} else {
				prev_chance += chance;
			}
		}
	}

	bool replaceable(std::string symb) {
		return cfg.has_rules(symb);
	}
	
	TiXmlElement* to_xml() {
		TiXmlElement* elem = new TiXmlElement("NORMAL_REPLACOR");
		TiXmlElement* cfg_elem = cfg.to_xml();
		elem->LinkEndChild(cfg_elem);
		return elem;
	}
	
private:
	std::mt19937 mt;
};

