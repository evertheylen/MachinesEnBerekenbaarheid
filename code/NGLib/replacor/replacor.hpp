
/* [bake me]

dependencies["headers"] = [
	"MBLib/new_CFG>>headers"
]

[stop baking] */

#pragma once

#include <string>
#include <vector>
#include <list>

#include "MBLib/new_CFG/CFG.hpp"
#include "../../libs/tinyxml/tinyxml.h"

template <typename RuleT>
class Replacor {
public:
	using Rule_Type = RuleT;
	
	// a list so popping and pushing is easy
	// TODO: string refs look hackerish and error-prone
	virtual RuleT replace(std::string var, std::list<RuleT>& context) = 0;
	
	virtual bool replaceable(std::string symb) = 0;
	// in a CFG bounded class, this would be `return is_var(var)`
	// this function decides whether a symbol will be replaced or outputted
	
	virtual TiXmlElement* to_xml() = 0;
};

// Dummy replacor
class DumbReplacor: public Replacor<SimpleRule<std::string>> {
public:
	using Rule_Type = SimpleRule<std::string>;
	
	SimpleRule<std::string> replace(std::string var, std::list<SimpleRule<std::string>>& context) {
		return SimpleRule<std::string>(var, {"(", var, ")"});
	}

	bool replaceable(std::string symb) {
		if (symb == "(" or symb == ")") {
			return false;
		}
		return true;
	}
	
	TiXmlElement* to_xml() {
		TiXmlElement* elem = new TiXmlElement("DUMB_REPLACOR");
		return elem;
	}
};

