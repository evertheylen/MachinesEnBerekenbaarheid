
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
#include "tinyxml/tinyxml.h"
#include "NGLib/tree/tree.hpp"
#include "NGLib/teacher/teacher.hpp"

// ContextReplacor is short for Context-Sensitive Stochastic Replacor using tables
class ContextReplacor: public CfgReplacor<ContextRule> {
public:
	using RuleT = ContextRule;
	
	ContextReplacor() = default;
	
	ContextReplacor(TiXmlElement* elem, std::random_device::result_type seed):
			CfgReplacor(elem->FirstChildElement()), mt(seed) {  // TODO
		for (auto& it: cfg.m_rules) {
			for (auto& it_2: cfg.m_rules) {
				it.second.fill_table(it_2.second.get_num());
			}
		}
	}
	
	unsigned int replace(std::string var, std::list<unsigned int>& context) {
// 		std::cout << "    --> replacing " << var << " with context: ";
// 		for (auto i: context) std::cout << i;
// 		std::cout << "\n";
		
		// get possible rules
		// pos_r is implicitly defined in cfg->rules(var), see later
		
		// build up C function, defining the chance for each rule
		std::map<unsigned int, double> C;
		
		// for q in pos_r
		for (auto q: cfg.rules(var)) {
			unsigned int q_num = q.second;
			C[q_num] = 1.0;
			// C[q] = product of all the chances of all the rules in the context
			for (auto u: context) {
				//std::cout << "trying to find " << q_num << " in table of " << u << "\n";
				assert(cfg.get_rule(u).table.find(q_num) != cfg.get_rule(u).table.end());
				C[q_num] *= cfg.get_rule(u).table.find(q_num)->second;
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
		unsigned int picked_rule;
		double temp_total = 0;
		for (auto& q: C) {
			temp_total += q.second;
			if (pick < temp_total) {
				picked_rule = q.first;
				break;
			}
		}
		
		// return stuff
		return picked_rule;
	}
	
	bool replaceable(std::string symb) {
		return cfg.has_rules(symb);
	}
	
	TiXmlElement* to_xml() {
		TiXmlElement* elem = new TiXmlElement("CONTEXT_REPLACOR");
		TiXmlElement* cfg_elem = cfg.to_xml();
		elem->LinkEndChild(cfg_elem);
		return elem;
	}
	
	void score(Teacher::Teacher3& tree, double score_amount) {
		std::set<Teacher::Element3> already_updated;
		score_helper(tree, score_amount, already_updated);
	}
	
private:
	void score_helper(Teacher::Teacher3& tree, double score_amount, std::set<Teacher::Element3>& already_updated) {
		// modify scores for all children of tree.
		for (Teacher::Teacher3* child: tree.all_children()) {
			if (child->data.second == 0) continue;
			if (already_updated.find({tree.data.second, child->data.second}) != already_updated.end()) continue; 
			double result = cfg.get_rule(tree.data.second).table[child->data.second] * score_amount;
			cfg.get_rule(tree.data.second).table[child->data.second] = result;
			already_updated.insert({tree.data.second, child->data.second});
		}
		
		// Call score recursively for all children
		for (Teacher::Teacher3* child: tree.children) {
			score_helper(*child, score_amount, already_updated);
		}
	}
	
	
	std::mt19937 mt;
	friend class Teacher;
	
};

