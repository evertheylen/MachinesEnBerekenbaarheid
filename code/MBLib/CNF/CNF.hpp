/*
 * CNF.hpp
 *
 *  Created on: 17 Oct 2015
 *      Author: stijn
 */

#ifndef CNF_CNF_HPP_
#define CNF_CNF_HPP_

#include <set>
#include "MBLib/CFG/CFG.hpp"
#include <set>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>

template<typename CFG_Type>
std::set<typename CFG_Type::ID_Type> find_nullable_symbols(
		const CFG_Type& input_cfg) {
	std::set<typename CFG_Type::ID_Type> nullable_symbols;
	for (auto iter : input_cfg.P) {
		for (auto rule : iter.second) {
			if (input_cfg.is_epsilon(rule)) { // TODO Find a decent symbol for epsilon, for the moment: $
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
				for (unsigned int i = 0; i < rule.size(); i++) {
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

template<typename CFG_Type>
std::vector<typename CFG_Type::ID_Type> delete_nullables(
		std::vector<typename CFG_Type::ID_Type> rule, int count,
		std::vector<int> position, int location) {
	if (count >= 0) {
		std::vector<typename CFG_Type::ID_Type> new_rule;
		for (int i = 0; i < position.at(location); i++) {
			new_rule.push_back(rule.at(i));
		}
		for (int i = position.at(location) + 1; i < rule.size(); i++) { // Changed this from rule.size() - 1 to rule.size()
			new_rule.push_back(rule.at(i));
		}
		if (location == position.size() - 1) {
			location = 0;
		} else {
			location++;
		}
		count--;
		return delete_nullables<CFG_Type>(new_rule, count, position, location);
	} else {
		return rule;
	}
}

template<typename CFG_Type>
CFG_Type eliminate_epsilon_productions(const CFG_Type& input_cfg) {
	CFG_Type result_cfg = input_cfg;
	std::set<typename CFG_Type::ID_Type> nullable_symbols =
			find_nullable_symbols(input_cfg);
	for (auto iter : input_cfg.P) {
		for (auto rule : iter.second) {
			if (input_cfg.is_epsilon(rule)) { // delete rules with epsilon body.
				result_cfg.P.find(iter.first)->second.erase(rule);
			}
			std::vector<int> position_nullables;
			for (unsigned int i = 0; i < rule.size(); i++) {
				if (nullable_symbols.find(rule.at(i))
						!= nullable_symbols.end()) { // If there is a nullable symbol in the rule.
					position_nullables.push_back(i);
				}
			}
			for (unsigned int i = 0; i < position_nullables.size(); i++) {
				//if (i == rule.size() - 1) { // m == k then don't delete the nullables because a new epsilon-rule would be created.
				if (i == rule.size() - 1) {
					break;
				}
				for (int it = 0; it < position_nullables.size(); it++) {
					std::vector<typename CFG_Type::ID_Type> new_rule =
							delete_nullables<CFG_Type>(rule, i,
									position_nullables, it);
					if (result_cfg.P.find(iter.first)->second.find(new_rule)
							== result_cfg.P.find(iter.first)->second.end()) { // If the new rule isnt in there yet.
						result_cfg.P.find(iter.first)->second.insert(new_rule);
					}
				}
			}
		}
	}
	//result_cfg.T.erase(*s_CFG::EPSILON); // Remove the epsilon out of the list of terminals because WE KILLED THEM ALL ;) TODO Change if epsilon becomes an element of T
	return result_cfg;
}

template<typename CFG_Type>
std::set<std::pair<typename CFG_Type::ID_Type, typename CFG_Type::ID_Type>> find_unit_pairs(
		const CFG_Type& input_cfg) {
	std::set<std::pair<typename CFG_Type::ID_Type, typename CFG_Type::ID_Type>> unit_pairs;
	for (auto variable : input_cfg.V) {
		unit_pairs.insert( { variable, variable });
	}

	bool added = true;
	while (added) {
		added = false;
		for (auto pair : unit_pairs) {
			auto iter = input_cfg.P.find(pair.second)->second;
			for (auto rule : iter) {
				if (rule.size() == 1
						&& input_cfg.V.find(rule.at(0)) != input_cfg.V.end()) {	// If the rule is a unit production
					if (unit_pairs.find( { pair.first, rule.at(0) })
							== unit_pairs.end()) { // If the rule is not in the unit pairs.
						unit_pairs.insert( { pair.first, rule.at(0) });
						added = true;
					}
				}
			}
		}
	}
	return unit_pairs;
}

template<typename CFG_Type>
CFG_Type eliminate_unit_pairs(const CFG_Type& input_cfg) {
	CFG_Type result_cfg = input_cfg;
	result_cfg.P.clear();
	std::set<std::pair<typename CFG_Type::ID_Type, typename CFG_Type::ID_Type>> unit_pairs =
			find_unit_pairs(input_cfg);
	for (auto pair : unit_pairs) {
		//std::cout << "pair: " << pair.first << ", " << pair.second << std::endl;
		typename CFG_Type::ID_Type new_head = pair.first;
		auto iter = input_cfg.P.find(pair.second)->second;
		//std::cout << "New rules:\n";
		for (auto rule : iter) {
			if (rule.size() != 1
					|| input_cfg.V.find(rule.at(0)) == input_cfg.V.end()) { // If the rule is not a variable only (this will create a new unit production).
				if (result_cfg.P.find(new_head) == result_cfg.P.end()) {
					result_cfg.P.insert(
							{ new_head, std::set<
									std::vector<typename CFG_Type::ID_Type>>() });
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

template<typename CFG_Type>
std::set<typename CFG_Type::ID_Type> find_generating_symbols(
		const CFG_Type& input_cfg) {
	std::set<typename CFG_Type::ID_Type> generating_symbols;
	for (typename CFG_Type::ID_Type terminal : input_cfg.T) {
		generating_symbols.insert(terminal);
	}

	bool added = true;
	while (added) {
		added = false;
		for (auto iter : input_cfg.P) {
			for (auto rule : iter.second) {
				bool add = true;
				for (unsigned int i = 0; i < rule.size(); i++) {
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

template<typename CFG_Type>
std::set<typename CFG_Type::ID_Type> find_reachable_symbols(
		const CFG_Type& input_cfg) {
	std::set<typename CFG_Type::ID_Type> reachable_symbols;
	reachable_symbols.insert(input_cfg.S);
	bool added = true;
	while (added) {
		added = false;
		for (typename CFG_Type::ID_Type reachable_symbol : reachable_symbols) {
			if (input_cfg.T.find(reachable_symbol) == input_cfg.T.end()) {
				auto rules = input_cfg.get_bodies(reachable_symbol);
				for (auto rule : rules) {
					for (unsigned int i = 0; i < rule.size(); i++) {
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

template<typename CFG_Type>
CFG_Type eliminate_useless_symbols(const CFG_Type& input_cfg) {
	CFG_Type result_cfg = input_cfg;
	std::set<typename CFG_Type::ID_Type> generating_symbols =
			find_generating_symbols(input_cfg);

	for (typename CFG_Type::ID_Type variable : input_cfg.V) {
		if (generating_symbols.find(variable) == generating_symbols.end()) { // If a variable isnt generating, then delete it and all of its rules.
			result_cfg.P.erase(variable);
			result_cfg.V.erase(variable);
		}
	}

	if (generating_symbols.find(input_cfg.S) == generating_symbols.end()) {
		//result_cfg.S = *s_CFG::EPSILON; TODO
	}

	std::set<typename CFG_Type::ID_Type> reachable_symbols =
			find_reachable_symbols(result_cfg);

	for (typename CFG_Type::ID_Type variable : result_cfg.V) {
		if (reachable_symbols.find(variable) == reachable_symbols.end()) {
			result_cfg.P.erase(variable);
			result_cfg.V.erase(variable);
		}
	}

	for (typename CFG_Type::ID_Type terminal : result_cfg.T) {
		if (reachable_symbols.find(terminal) == reachable_symbols.end()) {
			result_cfg.T.erase(terminal);
		}
	}

	return result_cfg;
}

template<typename CFG_Type>
CFG_Type long_rules_to_only_variables(const CFG_Type& input_cfg) {
	CFG_Type result_cfg = input_cfg;
	typename CFG_Type::ID_Type terminal;
	typename CFG_Type::ID_Type variable;
	std::set<typename CFG_Type::ID_Type> replaced_terminals;
	for (auto iter : result_cfg.P) {
		for (auto rule : iter.second) {
			if (rule.size() >= 2) {
				/*std::cout << "Long rule:";
				 for (unsigned int i = 0; i < rule.size(); i++) {
				 std::cout << rule.at(i);
				 }
				 std::cout << std::endl;*/
				for (unsigned int i = 0; i < rule.size(); i++) {
					if (result_cfg.T.find(rule.at(i)) != result_cfg.T.end()
							&& replaced_terminals.find(rule.at(i))
									== replaced_terminals.end()) { // If a terminal appears in a body of length >= 2.
						std::string temp = rule.at(i);
						std::transform(rule.at(i).begin(), rule.at(i).end(),
								temp.begin(), toupper);
						variable = "[_" + temp + "]";
						result_cfg.V.insert(variable);
						terminal = rule.at(i);
						replaced_terminals.insert(terminal);
						for (auto iter2 : result_cfg.P) {
							for (auto rule2 : iter2.second) {
								if (rule2.size() >= 2) {
									std::vector<typename CFG_Type::ID_Type> new_rule;
									for (unsigned int j = 0; j < rule2.size();
											j++) {
										if (rule2.at(j) == terminal) {
											new_rule.push_back(variable);
										} else {
											new_rule.push_back(rule2.at(j));
										}
										/*std::cout << "Found the terminal "
										 << terminal << " in rule: ";
										 for (unsigned int k = 0;
										 k < rule2.size(); k++) {
										 std::cout << rule2.at(k);
										 }
										 std::cout << std::endl;
										 */
										/*for (unsigned int k = 0; k < j; k++) {
										 new_rule.push_back(rule2.at(k));
										 }
										 new_rule.push_back(variable);
										 for (unsigned int k = j + 1;
										 k < rule2.size(); k++) {
										 new_rule.push_back(rule2.at(k));
										 }
										 result_cfg.P.find(iter2.first)->second.erase(
										 rule2);
										 result_cfg.P.find(iter2.first)->second.insert(
										 new_rule);
										 */
									}
									result_cfg.P.find(iter2.first)->second.erase(
											rule2);
									result_cfg.P.find(iter2.first)->second.insert(
											new_rule);
								}
							}
						}
						result_cfg.P.insert(
								{ variable,
										std::set<
												std::vector<
														typename CFG_Type::ID_Type>>() });
						result_cfg.P.find(variable)->second.insert(
								{ terminal });
					}
				}
			}
		}
	}
	return result_cfg;
}

template<typename CFG_Type> CFG_Type break_long_bodies(
		const CFG_Type& input_cfg) {
	CFG_Type result_cfg = input_cfg;
	int counter = 0;
	for (auto iter : result_cfg.P) {
		for (auto rule : iter.second) {
			std::string variable_begin = "C";
			if (rule.size() >= 3) {
				typename CFG_Type::ID_Type head = iter.first;
				for (unsigned int i = 1; i < rule.size(); i++) {
					std::string variable;
					if (i != rule.size() - 1) {
						variable = "[" + variable_begin + "_"
								+ std::to_string(counter) + "]";
					} else {
						variable = rule.at(rule.size() - 1);
					}
					result_cfg.V.insert(variable);
					std::vector<typename CFG_Type::ID_Type> new_rule;
					new_rule.push_back(rule.at(i - 1));
					new_rule.push_back(variable);
					if (result_cfg.P.find(head) == result_cfg.P.end()) {
						result_cfg.P.insert(
								{ head,
										std::set<
												std::vector<
														typename CFG_Type::ID_Type>>() });
					}
					if (head == iter.first) {
						for (auto iter2 : result_cfg.P) {
							for (auto rule2 : iter2.second) {
								if (rule2 == rule) {
									result_cfg.P.find(iter2.first)->second.insert(
											new_rule);
									result_cfg.P.find(iter2.first)->second.erase(
											rule2);
								}
							}
						}
					}
					result_cfg.P.find(head)->second.insert(new_rule);
					head = variable;
					if (i != rule.size() - 1) {
						counter++;
					}
				}
				result_cfg.P.find(iter.first)->second.erase(rule);
			}
		}
	}

	return result_cfg;
}

template<typename CFG_Type>
CFG_Type CNF(const CFG_Type& input_cfg) {
	CFG_Type result_cfg = break_long_bodies(
			long_rules_to_only_variables(
					eliminate_useless_symbols(
							eliminate_unit_pairs(
									eliminate_epsilon_productions(
											input_cfg))))); // Cleanup the grammar. Eliminate terminals from long bodies and break the long bodies.
	return result_cfg;
}

#endif /* CNF_CNF_HPP_ */
