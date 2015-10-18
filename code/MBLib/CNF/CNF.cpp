/*
 * CNF.cpp
 *
 *  Created on: 17 Oct 2015
 *      Author: stijn
 */
#include "../CFG.hpp"
#include "CNF.hpp"
#include <set>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <utility>

CFG CNF(const CFG& input_cfg) {
	CFG result_cfg = eliminate_useless_symbols(eliminate_unit_pairs(eliminate_epsilon_productions(input_cfg))); // Cleanup the grammar.

	return result_cfg;
}

std::set<char> find_nullable_symbols(const CFG& input_cfg) {
	std::set<char> nullable_symbols;
	for (auto iter : input_cfg.P) {
		for (auto rule : iter.second) {
			if (rule == CFG::EPSILON) { // TODO Find a decent symbol for epsilon, for the moment: $
				if (nullable_symbols.find(iter.first) == nullable_symbols.end())
					nullable_symbols.insert(iter.first);
			}
		}
	}

	bool added_new_nullable = true;
	while (added_new_nullable) {
		added_new_nullable = false;
		for (auto iter : input_cfg.P) {
			for (auto rule : iter.second) {
				bool add = true;
				for (unsigned int i = 0; i < rule.length(); i++) {
					if (nullable_symbols.find(rule.at(i))
							== nullable_symbols.end()) {
						add = false;
						break;
					}
				}
				if (add) {
					if (nullable_symbols.find(iter.first)
							== nullable_symbols.end()) {
						nullable_symbols.insert(iter.first);
						added_new_nullable = true;
					}
				}
			}
		}
	}
	return nullable_symbols;
}

CFG eliminate_epsilon_productions(const CFG& input_cfg) {
	CFG result_cfg = input_cfg;
	std::set<char> nullable_symbols = find_nullable_symbols(input_cfg);
	for (auto iter : input_cfg.P) {
		for (auto rule : iter.second) {
			if (rule == CFG::EPSILON) {		// delete rules with epsilon body.
				result_cfg.P.find(iter.first)->second.erase(rule);
			}
			std::vector<int> position_nullables;
			for (unsigned int i = 0; i < rule.length(); i++) {
				if (nullable_symbols.find(rule.at(i))
						!= nullable_symbols.end()) { // If there is a nullable symbol in the rule.
					position_nullables.push_back(i);
				}
			}
			for (unsigned int i = 0; i < position_nullables.size(); i++) {
				if (i == rule.length() - 1) { // m == k then don't delete the nullables because a new epsilon-rule would be created.
					break;
				}
				for (std::vector<int>::iterator it = position_nullables.begin();
						it != position_nullables.end(); it++) {
					std::string new_rule = delete_nullables(rule, i,
							position_nullables, it);
					if (result_cfg.P.find(iter.first)->second.find(new_rule)
							== result_cfg.P.find(iter.first)->second.end()) { // If the new rule isnt in there yet.
						result_cfg.P.find(iter.first)->second.insert(new_rule);
					}
				}
			}
		}
	}
	//std::cout << result_cfg;
	result_cfg.T.erase(*CFG::EPSILON); // Remove the epsilon out of the list of terminals because WE KILLED THEM ALL ;)
	return result_cfg;
}

std::string delete_nullables(std::string rule, int count,
		std::vector<int> position, std::vector<int>::iterator it) {
	if (count >= 0) {
		std::string new_rule = rule.substr(0, *it);
		if (*it + 1 < rule.length()) {
			new_rule += rule.substr(*it + 1, rule.length() - 1);
		}
		std::vector<int>::iterator new_it;
		if (it == position.end() - 1) {
			new_it = position.begin();
		} else {
			new_it = it++;
		}
		count--;
		return delete_nullables(new_rule, count, position, new_it);
	}
	if (count < 0) {
		return rule;
	}
}

std::set<std::pair<char, char>> find_unit_pairs(const CFG& input_cfg) {
	std::set<std::pair<char, char>> unit_pairs;
	for (auto variable : input_cfg.V) {
		unit_pairs.insert(std::pair<char, char>(variable, variable));
	}

	bool added = true;
	while (added) {
		added = false;
		for (auto pair : unit_pairs) {
			auto iter = input_cfg.P.find(pair.second)->second;
			for (auto rule : iter) {
				if (rule.length() == 1
						&& input_cfg.V.find(rule.at(0)) != input_cfg.V.end()) {	// If the rule is a unit production
					if (unit_pairs.find(
							std::pair<char, char>(pair.first, rule.at(0)))
							== unit_pairs.end()) { // If the rule is not in the unit pairs.
						unit_pairs.insert(
								std::pair<char, char>(pair.first, rule.at(0)));
						added = true;
					}
				}
			}
		}
	}
	return unit_pairs;
}

CFG eliminate_unit_pairs(const CFG& input_cfg) {
	CFG result_cfg = input_cfg;
	result_cfg.P.clear();
	std::set<std::pair<char, char>> unit_pairs = find_unit_pairs(input_cfg);
	for (auto pair : unit_pairs) {
		//std::cout << "pair: " << pair.first << ", " << pair.second << std::endl;
		char new_head = pair.first;
		auto iter = input_cfg.P.find(pair.second)->second;
		//std::cout << "New rules:\n";
		for (auto rule : iter) {
			if (rule.length() != 1
					|| input_cfg.V.find(rule.at(0)) == input_cfg.V.end()) { // If the rule is not a variable only (this will create a new unit production).
				if (result_cfg.P.find(new_head) == result_cfg.P.end()) {
					result_cfg.P.insert(
							std::pair<char, std::set<std::string>>(new_head,
									std::set<std::string>()));
				}
				if (result_cfg.P.find(new_head)->second.find(rule)
						== result_cfg.P.find(new_head)->second.end()) { // if the rule doesn't exist yet.
					result_cfg.P.find(new_head)->second.insert(rule);
					//std::cout << pair.first << " --> " << rule << std::endl;
				}
			}
		}
	}
	return result_cfg;
}

std::set<char> find_generating_symbols(const CFG& input_cfg) {
	std::set<char> generating_symbols;
	for (char terminal : input_cfg.T) {
		generating_symbols.insert(terminal);
	}

	bool added = true;
	while (added) {
		added = false;
		for (auto iter : input_cfg.P) {
			for (auto rule : iter.second) {
				bool add = true;
				for (unsigned int i = 0; i < rule.length(); i++) {
					if (generating_symbols.find(rule.at(i))
							== generating_symbols.end()) {
						add = false;
						break;
					}
				}
				if (add) {
					if (generating_symbols.find(iter.first)
							== generating_symbols.end()) {
						generating_symbols.insert(iter.first);
						added = true;
					}
				}
			}
		}
	}

	return generating_symbols;
}

std::set<char> find_reachable_symbols(const CFG& input_cfg) {
	std::set<char> reachable_symbols;
	reachable_symbols.insert(input_cfg.S);

	bool added = true;
	while (added) {
		added = false;
		for (char reachable_symbol : reachable_symbols) {
			if (input_cfg.T.find(reachable_symbol) == input_cfg.T.end()) {
				auto rules = input_cfg.P.find(reachable_symbol)->second;
				for (auto rule : rules) {
					for (unsigned int i = 0; i < rule.length(); i++) {
						if (reachable_symbols.find(rule.at(i))
								== reachable_symbols.end()) {
							reachable_symbols.insert(rule.at(i));
							added = true;
						}
					}
				}
			}
		}
	}
	return reachable_symbols;
}

CFG eliminate_useless_symbols(const CFG& input_cfg) {
	CFG result_cfg = input_cfg;
	std::set<char> generating_symbols = find_generating_symbols(input_cfg);

	for (char variable : input_cfg.V) {
		if (generating_symbols.find(variable) == generating_symbols.end()) { // If a variable isnt generating, then delete it and all of its rules.
			result_cfg.P.erase(variable);
			result_cfg.V.erase(variable);
		}
	}

	if (generating_symbols.find(input_cfg.S) == generating_symbols.end()) {
		result_cfg.S = *CFG::EPSILON;
	}

	std::set<char> reachable_symbols = find_reachable_symbols(result_cfg);

	for (char variable : result_cfg.V) {
		if (reachable_symbols.find(variable) == reachable_symbols.end()) {
			result_cfg.P.erase(variable);
			result_cfg.V.erase(variable);
		}
	}

	for (char terminal : result_cfg.T) {
		if (reachable_symbols.find(terminal) == reachable_symbols.end()) {
			result_cfg.T.erase(terminal);
		}
	}

	return result_cfg;
}

CFG long_rules_to_only_variables(const CFG& input_cfg) {
	for (auto iter: input_cfg.P) {
		for (auto rule: iter.second) {
			if (rule.length() >= 2) {
				for (unsigned int i = 0; i < rule.length(); i++) {
					if (input_cfg.T.find(rule.at(i)) != input_cfg.T.end()) { // If a terminal appears in a body of length >= 2.

					}
				}
			}
		}
	}
}

CFG break_long_bodies(const CFG& input_cfg) {

}
