/*
[bake me]

dependencies["headers"] = [
	"libs/tinyxml>>headers",
	"MBLib/common>>headers",
	"MBLib/new_CFG>>headers",
]

[stop baking]

*/

#ifndef STOCHASTIC_RULE
#define STOCHASTIC_RULE

#include <set>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <cassert>

#include "libs/tinyxml/tinyxml.h"
#include "MBLib/common/common.hpp"
#include "MBLib/new_CFG/CFG.hpp"

template <typename ID_T>
class StochasticRule: public SimpleRule<ID_T> {
public:
	using ID_Type = ID_T;
	using StringT = std::vector<ID_T>;
	
	StochasticRule(const ID_T& _head, const std::vector<ID_T>& _body, double _chance):
		SimpleRule(_head, _body), chance(_chance) {}
	
	double get_chance() const {
	    return chance;
	}
	double chance;
};


#endif //STOCHASTIC_RULE
