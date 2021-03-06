
#pragma once

#include <string>
#include <vector>
#include <list>

#include "tinyxml/tinyxml.h"

#include "MBLib/new_CFG/CFG.hpp"
#include "NGLib/teacher/teacher.hpp"

class Replacor {
public:
	
	Replacor() = default;
	
	// a list so popping and pushing is easy
	virtual unsigned int replace(const std::string& var, std::list<unsigned int>& context) = 0;
	
	virtual bool replaceable(const std::string& symb) = 0;
	// in a CFG bounded class, this would be `return is_var(var)`
	// this function decides whether a symbol will be replaced or outputted
	
	virtual TiXmlElement* to_xml() = 0;
	
	virtual void score(Teacher::Teacher3& tree, double score_amount);
	
	virtual const std::vector<std::string>& get_body(unsigned int num) const = 0;
	
	virtual ~Replacor() {}
	
};


template <typename _RuleT>
class CfgReplacor: public Replacor {
public:
	CfgReplacor() = default;
	
	CfgReplacor(TiXmlElement* root): cfg(root) {}
	
	const std::vector<std::string>& get_body(unsigned int num) const {
		return cfg.get_rule_c(num).get_body();
	}
	
	bool replaceable(const std::string& symb) {
		return cfg.has_rules(symb);
	}
	
	using RuleT = _RuleT;
	
protected:
	xml_CFG<RuleT> cfg;
};

