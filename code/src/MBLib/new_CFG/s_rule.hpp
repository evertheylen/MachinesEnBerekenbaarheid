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

class StochasticRule: public SimpleRule {
public:
	
	StochasticRule() = default;
	
	StochasticRule(TiXmlElement* root): SimpleRule(root) {
		const char* chance_attr = root->Attribute("chance");
		if (chance_attr == nullptr) throw SyntacticError("No chance attr");
		chance = std::stod(std::string(chance_attr));
	}
	
	StochasticRule(const std::string& _head, const std::vector<std::string>& _body, unsigned int _num, double _chance):
		SimpleRule(_head, _body, _num), chance(_chance) {}
	
	TiXmlElement* to_xml() {
		TiXmlElement* root = SimpleRule::to_xml();
		// more precision for chance
		std::stringstream morePrecision;
		morePrecision.precision(20);
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
