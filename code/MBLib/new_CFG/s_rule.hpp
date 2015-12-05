/*
[bake me]

dependencies["headers"] = [
	"libs/tinyxml>>headers",
	"MBLib/common>>headers",
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
	
	StochasticRule(TiXmlElement* root): SimpleRule<ID_T>(root) {
		chance = std::stod(std::string(root->Attribute("chance")));
	}
	
	StochasticRule(const ID_T& _head, const std::vector<ID_T>& _body, double _chance):
		SimpleRule<ID_T>(_head, _body), chance(_chance) {}
	
	TiXmlElement* to_xml() {
		TiXmlElement* root = SimpleRule<ID_T>::to_xml();
		root->SetAttribute("chance", std::to_string(chance));
		return root;
	}
	
	double get_chance() const {
	    return chance;
	}
	
	double chance;
};


#endif //STOCHASTIC_RULE
