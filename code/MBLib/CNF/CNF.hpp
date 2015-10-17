/*
 * CNF.hpp
 *
 *  Created on: 17 Oct 2015
 *      Author: stijn
 */

#ifndef CNF_CNF_HPP_
#define CNF_CNF_HPP_

#include <set>
#include "../CFG.hpp"
#include <vector>
#include <string>

CFG CNF(const CFG& input_cfg);

std::set<char> find_nullable_symbols(const CFG& input_cfg);

CFG eliminate_epsilon_productions(const CFG& input_cfg);

std::string delete_nullables(std::string rule, int count,
		std::vector<int> position, std::vector<int>::iterator it);

#endif /* CNF_CNF_HPP_ */
