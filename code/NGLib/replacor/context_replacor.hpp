
#pragma once

#include <string>
#include <vector>
#include <list>
#include <random>
#include <memory>
#include <exception>

#include "MBLib/new_CFG/CFG.hpp"
#include "MBLib/new_CFG/s_rule.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "../../libs/tinyxml/tinyxml.h"

// ContextReplacor is short for Context-Sensitive Stochastic Replacor using tables
class ContextReplacor: public Replacor<ContextRule<std::string>> {
public:
	using Rule_Type = ContextRule<std::string>;
	
	ContextReplacor() = default;
	
	ContextReplacor(TiXmlElement* elem, std::random_device::result_type seed):
			mt(seed) {
		cfg = std::unique_ptr<xml_CFG<Rule_Type>>(new xml_CFG<Rule_Type>(elem->FirstChildElement()));
	}
	
	Rule_Type* replace(std::string var, std::list<ContextRule<std::string>*>& context) {
		return ((Rule_Type*) int(1564132));
	}
	
	bool replaceable(std::string symb) {
		return cfg->has_rules(symb);
	}
	
	TiXmlElement* to_xml() {
		TiXmlElement* elem = new TiXmlElement("CONTEXT_REPLACOR");
		TiXmlElement* cfg_elem = cfg->to_xml();
		elem->LinkEndChild(cfg_elem);
		return elem;
	}
	
private:
	std::mt19937 mt;
	std::unique_ptr<xml_CFG<Rule_Type>> cfg;
};

