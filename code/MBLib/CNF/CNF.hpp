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

CFG CNF(const CFG& input_cfg);

std::set<char> find_nullable_symbols(const CFG& input_cfg);



#endif /* CNF_CNF_HPP_ */
