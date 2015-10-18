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

CFG CNF(const CFG& input_cfg) {

}

std::set<char> find_nullable_symbols(const CFG& input_cfg) {
	std::set<char> nullable_symbols;
	for (auto iter : input_cfg.P) {
		for (auto rule : iter.second) {
			if (rule == "$") { // TODO Find a decent symbol for epsilon, for the moment: $
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
	/*for (auto symbol: nullable_symbols) {
	 std::cout << "Nullable symbols " << symbol << std::endl;
	 }*/
	return nullable_symbols;
}

CFG eliminate_epsilon_productions(const CFG& input_cfg) {
	CFG result_cfg = input_cfg;
	std::set<char> nullable_symbols = find_nullable_symbols(input_cfg);
	for (auto iter : input_cfg.P) {
		for (auto rule : iter.second) {
			if (rule == "$") {		// delete rules with epsilon body.
				result_cfg.P.find(iter.first)->second.erase(rule);
			}
			std::vector<int> position_nullables;
			for (unsigned int i = 0; i < rule.length(); i++) {
				if (nullable_symbols.find(rule.at(i))
						!= nullable_symbols.end()) { // If there is a nullable symbol in the rule.
					position_nullables.push_back(i);
					/*std::string new_rule = rule.substr(0,i) + rule.substr(i+1, rule.length()-1);
					 std::cout << " new rule: " << new_rule << std::endl;
					 if ()
					 if (result_cfg.P.find(iter.first)->second.find(new_rule) == result_cfg.P.find(iter.first)->second.end()) { // If the new rule isnt in there yet.
					 result_cfg.P.find(iter.first)->second.insert(new_rule);
					 }*/
				}
			}
			for (unsigned int i = 0; i < position_nullables.size(); i++) {
				if (i == rule.length() - 1) {
					break;
				}
				for (std::vector<int>::iterator it = position_nullables.begin();
						it != position_nullables.end(); it++) {
					std::string new_rule = delete_nullables(rule, i,
							position_nullables, it);
					//std::cout << new_rule << std::endl;
					if (result_cfg.P.find(iter.first)->second.find(new_rule)
							== result_cfg.P.find(iter.first)->second.end()) { // If the new rule isnt in there yet.
						result_cfg.P.find(iter.first)->second.insert(new_rule);
					}
				}
			}

			// TODO m==k dont delete all the nullable symbols.
		}
	}

	std::cout << result_cfg;
	return result_cfg;
}

std::string delete_nullables(std::string rule, int count,
		std::vector<int> position, std::vector<int>::iterator it) {
	if (count >= 0) {
		std::string new_rule = rule.substr(0, *it);
		if (*it+1 < rule.length()) {
			new_rule += rule.substr(*it+1, rule.length()-1);
		}
		//std::cout << new_rule << std::endl;
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
