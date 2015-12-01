
/* [bake me]

dependencies["headers"] = [
	"MBLib/new_CFG>>headers",
	"NGLib/common>>headers",
	"NGLib/replacor>>headers",
]

[stop baking] */

#pragma once

#include <string>
#include <vector>
#include <list>

#include "MBLib/new_CFG/CFG.hpp"
#include "NGLib/common/s_rule.hpp"

// Stochastic replacor
class StochasticReplacor: public Replacor<StochasticRule<std::string>> {
public:
	using Rule_Type = StochasticRule<std::string>;
	
	Rule_Type replace(std::string var, std::list<SimpleRule<std::string>>& context) {
	    // TODO Fix this.
	    
	    // Alle regels van een bepaalde var worden opgevraagd. Alle kansen worden samengegooid. Op basis van deze kansen wordt de regel gekozen. Deze regel wordt gereturned.
	
        cfg.rules(var);
	
		return Rule_Type(var, {"(", var, ")"}, chance);
	}

	bool replaceable(std::string symb) {
		return cfg.has_rules(symb);
	}

};

