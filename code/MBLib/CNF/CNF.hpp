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
#include <vector>
#include <utility>
#include <string>

template <typename CFG_Type> CFG_Type CNF(const CFG_Type& input_cfg);

template <typename CFG_Type> std::set<char> find_nullable_symbols(const CFG_Type& input_cfg);

template <typename CFG_Type> CFG_Type eliminate_epsilon_productions(const CFG_Type& input_cfg);

std::string delete_nullables(std::string rule, int count,
		std::vector<int> position, std::vector<int>::iterator it);

template <typename CFG_Type> std::set<std::pair<char, char>> find_unit_pairs(const CFG_Type& input_cfg);

template <typename CFG_Type> CFG_Type  eliminate_unit_pairs(const CFG_Type& input_cfg);

template <typename CFG_Type> std::set<char> find_generating_symbols(const CFG_Type& input_cfg);

template <typename CFG_Type> std::set<char> find_reachable_symbols(const CFG_Type& input_cfg);

template <typename CFG_Type> CFG_Type  eliminate_useless_symbols(const CFG_Type& input_cfg);

template <typename CFG_Type> CFG_Type  long_rules_to_only_variables(const CFG_Type& input_cfg);

template <typename CFG_Type> CFG_Type  break_long_bodies(const CFG_Type& input_cfg);

#endif /* CNF_CNF_HPP_ */
