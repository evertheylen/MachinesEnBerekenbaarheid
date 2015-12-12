
#pragma once

#include <string>
#include <vector>
#include <list>
#include <random>
#include <memory>
#include <exception>

#include "MBLib/new_CFG/CFG.hpp"
#include "MBLib/new_CFG/cs_rule.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "../../libs/tinyxml/tinyxml.h"

// ContextReplacor is short for Context-Sensitive Stochastic Replacor using tables
class ContextReplacor: public Replacor<ContextRule<std::string>> {
public:
	using Rule_Type = ContextRule<std::string>;
	using NumT = typename Rule_Type::NumT;
	
	ContextReplacor() = default;
	
	ContextReplacor(TiXmlElement* elem, std::random_device::result_type seed):
			mt(seed) {
		cfg = std::unique_ptr<xml_CFG<Rule_Type>>(new xml_CFG<Rule_Type>(elem->FirstChildElement()));
	}
	
	std::vector<typename Rule_Type::ID_Type> replace(std::string var, std::list<typename Rule_Type::NumT>& context) {
// 		std::cout << "    --> replacing " << var << " with context: ";
// 		for (auto i: context) std::cout << i;
// 		std::cout << "\n";
		
		// get possible rules
		// pos_r is implicitly defined in cfg->rules(var), see later
		
		// build up C function, defining the chance for each rule
		std::map<NumT, double> C;
		
		// for q in pos_r
		for (auto q: cfg->rules(var)) {
			NumT& q_num = q.second;
			C[q_num] = 1.0;
			// C[q] = product of all the chances of all the rules in the context
			for (auto u: context) {
				//std::cout << "trying to find " << q_num << " in table of " << u << "\n";
				assert(cfg->get_rule(u).table.find(q_num) != cfg->get_rule(u).table.end());
				C[q_num] *= cfg->get_rule(u).table.find(q_num)->second;
			}
		}
		
		// pick from C
		double total = 0.0;
		for (auto& q: C) {
			//std::cout << "C contains " << q.first << "\n";
			total += q.second;
		}
		std::uniform_real_distribution<double> dist;
		double pick = dist(mt) * total;
		NumT picked_rule;
		double temp_total = 0;
		for (auto& q: C) {
			temp_total += q.second;
			if (pick < temp_total) {
				picked_rule = q.first;
				break;
			}
		}
		
		// modify context
		context.push_back(picked_rule);
		
		// return stuff
		return cfg->get_rule(picked_rule).get_body();
	}
	
	bool replaceable(std::string symb) {
		return cfg->has_rules(symb);
	}
	
	TiXmlElement* to_xml() {
		TiXmlElement* elem = new TiXmlElement("CONTEXT_REPLACOR");
		TiXmlElement* cfg_elem = cfg->to_xml();
		elem->LinkEndChild(cfg_elem);
		return elem;
	}
	
private:
	std::mt19937 mt;
	std::unique_ptr<xml_CFG<Rule_Type>> cfg;
};

