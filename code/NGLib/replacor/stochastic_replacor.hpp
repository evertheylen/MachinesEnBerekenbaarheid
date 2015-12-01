
#pragma once

#include <string>
#include <vector>
#include <list>
#include <random>

#include "MBLib/new_CFG/CFG.hpp"
#include "MBLib/new_CFG/s_rule.hpp"
#include "NGLib/replacor/replacor.hpp"

// Stochastic replacor
class StochasticReplacor: public Replacor<StochasticRule<std::string>> {
public:
	using Rule_Type = StochasticRule<std::string>;
	
	StochasticReplacor(CFG<Rule_Type> _cfg, std::random_device::result_type seed): cfg(_cfg), mt(seed) {}
	
	Rule_Type replace(std::string var, std::list<SimpleRule<std::string>>& context) {
		std::uniform_int_distribution<int> dist(0, 99);
		int picked_rule = dist(mt);
		double prev_chance = 0;
		for (auto it: cfg.rules(var)) {
			double chance = it.second.get_chance();
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
	
private:
	CFG<Rule_Type> cfg;
	std::mt19937 mt;
};

