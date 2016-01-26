#include <vector>

#include "context_replacor.hpp"

ContextReplacor::ContextReplacor(TiXmlElement* elem, std::random_device::result_type seed):
	CfgReplacor(elem->FirstChildElement()) {}


unsigned int ContextReplacor::replace(const std::string& var, std::list<unsigned int>& context) {
//	std::cout << "    --> replacing " << var << " with context: ";
//	for (auto i: context) std::cout << i;
// 	std::cout << "\n";
	
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
			if (cfg.get_rule(u).table.find(q_num) != cfg.get_rule(u).table.end()) {
				C[q_num] *= cfg.get_rule(u).table.find(q_num)->second;
			}
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


TiXmlElement* ContextReplacor::to_xml() {
	TiXmlElement* elem = new TiXmlElement("CONTEXT_REPLACOR");
	TiXmlElement* cfg_elem = cfg.to_xml();
	elem->LinkEndChild(cfg_elem);
	return elem;
}

void ContextReplacor::score(Teacher::Teacher3& tree, double score_amount) {
	std::set<ScoreEl> already_updated;
	score_helper(tree, score_amount, already_updated);
}

void ContextReplacor::score_helper(Teacher::Teacher3& tree, double score_amount, std::set<ScoreEl>& already_updated) {
	// modify scores for all children of tree.
	for (Teacher::Teacher3* child: tree.all_children()) {
		if (child->data.second == 0) continue;
		if (already_updated.find({tree.data.second, child->data.second}) != already_updated.end()) continue; 
		
		RuleT& rule = cfg.get_rule(tree.data.second);
		// fill in with 1.0 if it doesn't exist
		if (rule.table.find(child->data.second) == rule.table.end()) rule.table[child->data.second] = 1.0;
		
		rule.table[child->data.second] *= score_amount;
		already_updated.insert({tree.data.second, child->data.second});
	}
	// Call score recursively for all children
	for (Teacher::Teacher3* child: tree.children) {
		score_helper(*child, score_amount, already_updated);
	}
}
