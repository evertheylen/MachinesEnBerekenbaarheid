
#pragma once

#include <string>
#include <list>
#include <random>
#include <memory>
#include <exception>

#include "MBLib/new_CFG/CFG.hpp"
#include "MBLib/new_CFG/cs_rule.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "tinyxml/tinyxml.h"
#include "NGLib/tree/tree.hpp"
#include "NGLib/teacher/teacher.hpp"

// ContextReplacor is short for Context-Sensitive Stochastic Replacor using tables
class ContextReplacor: public CfgReplacor<ContextRule> {
public:
	using RuleT = ContextRule;
	
	ContextReplacor() = default;
	
	ContextReplacor(TiXmlElement* elem, std::random_device::result_type seed);
	
	unsigned int replace(std::string var, std::list<unsigned int>& context);
	
	bool replaceable(std::string symb);
	
	TiXmlElement* to_xml();
	
	void score(Teacher::Teacher3& tree, double score_amount);
	
private:
	void score_helper(Teacher::Teacher3& tree, double score_amount, std::set<Teacher::Element3>& already_updated);
	
	std::mt19937 mt;
	friend class Teacher;
	
};

