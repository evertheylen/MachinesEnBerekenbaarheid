
/* [bake me]

dependencies["headers"] = [
	"MBLib/new_CFG>>headers",
	"NGLib/common>>headers",
	"NGLib/replacor>>headers",
]

[stop baking] */

#pragma once

#include <string>
#include <vector>
#include <list>
#include <random>

#include "MBLib/new_CFG/CFG.hpp"
#include "MBLib/common/s_rule.hpp"

// Stochastic replacor
class StochasticReplacor: public Replacor<StochasticRule<std::string>> {
public:
	using Rule_Type = StochasticRule<std::string>;
	
	StochasticReplacor(CFG<Rule_Type> _cfg, std::random_device::result_type seed): cfg(_cfg), mt(seed) {}
	
	Rule_Type replace(std::string var, std::list<SimpleRule<std::string>>& context) {
		std::uniform_int_distribution<int> dist(0, 99);
		int picked_rule = dist(mt);
		double prev_chance = 0;
		while(true) {
			Rule_Type rule = cfg.rules(var).second; 	//TODO This should iterate over the rules somehow
			double chance = rule.get_chance();
			if (picked_rule - prev_chance < chance*100) {
				return rule;
			} else {
				prev_chance += chance*100;
			}
		}
	}

	bool replaceable(std::string symb) {
		return cfg.has_rules(symb);
	}
	
private:
	CFG<Rule_Type> cfg;
	std::mt19937 mt;
};

