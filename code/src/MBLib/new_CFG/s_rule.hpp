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
#include <iomanip>
#include <sstream>

#include <cassert>

#include "libs/tinyxml/tinyxml.h"
#include "MBLib/common/common.hpp"
#include "MBLib/new_CFG/CFG.hpp"

template <typename ID_T>
class StochasticRule: public SimpleRule<ID_T> {
public:
	using ID_Type = ID_T;
	using StringT = std::vector<ID_T>;
	using NumT = unsigned int;
	
	StochasticRule() = default;
	
	StochasticRule(TiXmlElement* root): SimpleRule<ID_T>(root) {
		const char* chance_attr = root->Attribute("chance");
		if (chance_attr == nullptr) throw SyntacticError("No chance attr");
		chance = std::stod(std::string(chance_attr));
	}
	
	StochasticRule(const ID_T& _head, const std::vector<ID_T>& _body, NumT _num, double _chance):
		SimpleRule<ID_T>(_head, _body, _num), chance(_chance) {}
	
	TiXmlElement* to_xml() {
		TiXmlElement* root = SimpleRule<ID_T>::to_xml();
		// more precision for chance
		std::stringstream morePrecision;
		morePrecision.precision(10);
		morePrecision << chance;
		//std::cout << morePrecision.str() << std::endl;
		root->SetAttribute("chance", morePrecision.str());
		return root;
	}
	
	double get_chance() const {
	    return chance;
	}
	
	double chance;
};


#endif //STOCHASTIC_RULE
