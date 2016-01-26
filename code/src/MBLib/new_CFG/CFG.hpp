
#pragma once

#include <set>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <utility>
#include <cassert>

#include "libs/tinyxml/tinyxml.h"
#include "MBLib/common/common.hpp"
#include "NGLib/exceptions/exceptions.hpp"

class SimpleRule {
public:
	using StringT = std::vector<std::string>;
	
	SimpleRule() = default;
	
	SimpleRule(TiXmlElement* root);
	
	SimpleRule(const std::string& _head, const std::vector<std::string>& _body, unsigned int _num);
	
	const std::string& get_head() const;
	
	const std::vector<std::string>& get_body() const;
	
	unsigned int get_num() const;
	
	TiXmlElement* to_xml();
	
	std::string head;
	std::vector<std::string> body;
	unsigned int num;
};




template <typename RuleT>
class RuleIterator {
public:
	using MapT = typename std::multimap<std::string, unsigned int>;
	
	RuleIterator(const std::string& _var, const MapT& _P):
			var(_var), P(_P) {}
	
	typename MapT::const_iterator begin() {
		return P.equal_range(var).first;
	}
	
	typename MapT::const_iterator end() {
		return P.equal_range(var).second;
	}
	
	unsigned int size() {
		return P.count(var);
	}
	
private:
	const std::string& var;
	const MapT& P;
};


// Description of what RuleT should offer:
//   - std::stringype type declaration
//   - String type declaration
//   - const std::stringype& get_head() method
template <typename _RuleT>
class CFG {
public:
	using RuleT = _RuleT;
	using StringT = std::vector<std::string>;
	
	std::set<std::string> V;								// Variables
	std::set<std::string> T;  								// Terminals
	std::multimap<std::string, unsigned int> P;	// Productions
	std::map<unsigned int, RuleT> m_rules;	// store rules
	std::string S;											// Start symbol, \in V
	
	CFG() {}
	
	CFG(std::set<std::string> _V, std::set<std::string> _T, std::string _S):
			V(_V), T(_T), S(_S) {}
	
	bool is_epsilon(const std::vector<std::string>& s) const {
		return s.empty();
	}
	
	bool is_variable(const std::string& symbol) const {
		auto m = V.find(symbol);
		return m != V.end();
	}
	
	bool is_terminal(const std::string& symbol) const {
		auto m = T.find(symbol);
		return m != T.end();
	}
	
	bool has_rules(const std::string& var) const {
        return P.find(var) != P.end();
	}
	
	
	RuleT& get_rule(const unsigned int& num) {
		assert(m_rules.find(num) != m_rules.end());
		return m_rules.find(num)->second;
	}
	
	const RuleT& get_rule_c(const unsigned int& num) const {
		if (m_rules.find(num) == m_rules.end()) std::cout << "did not find " << num << "\n";
		return m_rules.find(num)->second;
	}
	
	template <typename Whatever>
	friend class RuleIterator;
	
	RuleIterator<RuleT> rules(const std::string& var) const {
		return RuleIterator<RuleT>(var, P);
	}
	
	void add_rule(const RuleT& rule) {
		assert(is_variable(rule.get_head()));
		assert(m_rules.find(rule.get_num()) == m_rules.end());
		//auto m = P.find(rule.get_head());
		
		//P.insert(std::pair<Rule_Type::std::string, Rule_Type::NumT>(rule.get_head(), rule.get_num()));
		P.insert({rule.get_head(), rule.get_num()});
		m_rules[rule.get_num()] = rule;
		
		assert(check_rule_correctness());
	}
	
	// assert this as much as you want
	bool check_rule_correctness() {
		for (auto& i: P) {
			if (i.first != get_rule(i.second).get_head()) {
				return false;
			}
		}
		return true;
	}
	
};


template <typename RuleT>
std::ostream& operator<< (std::ostream& out, CFG<RuleT>& G) {
	out << "Variables = " << G.V << "\n";
	out << "Terminals = " << G.T << "\n";
	
	out << "Productions = {\n";
	for (auto iter: G.P) {
		for (auto n: iter.second) {
			out << "  (" << n << "):  " << G.get_rule(n).get_head() << " --> " << to_string(G.get_rule(n).get_body()) << "\n";
		}
	}
	
	out << "}\nStart symbol = " << G.S << "\n";
	return out;
}


// xml_CFG requires more for RuleT:
//   - constructor RuleT(head, body)
//   - std::vector<std::stringype>& get_body() method
//   - string based (Basically, std::stringype should be std::string)
template <typename _RuleT>
class xml_CFG: public CFG<_RuleT> {
public:
	
	using RuleT = _RuleT;
	using Parent = CFG<RuleT>;
	
	// Hi my name is Bjarne Stroustrup and fuck you 'cause there is no easy mechanism for inheriting constructors
	// Did I tell you about typedefs? No easy way to inherit them either :D Have fun with C++! lol
	
	xml_CFG() {}

	xml_CFG(std::set<std::string> _V, std::set<std::string> _T, std::string _S):
			Parent(_V, _T, _S) {}

	xml_CFG(TiXmlElement* root) {
		auto vars_el = root->FirstChildElement("Variables");
		if (vars_el == nullptr) throw SyntacticError("No Variables element");
		for (auto s: split(std::string(vars_el->GetText()))) {
			this->V.insert(s);
		}
		this->S = std::string(vars_el->FirstChildElement("StartSymbol")->GetText());

		auto term_el = root->FirstChildElement("Terminals");
		if (term_el == nullptr) throw SyntacticError("No Terminals element");
		if (term_el->GetText() != nullptr) {
			for (auto s: split(std::string(term_el->GetText()))) {
				this->T.insert(s);
			}
		}
		
		auto prod_el = root->FirstChildElement("Productions");
		if (prod_el == nullptr) throw SyntacticError("No Productions element");
		for (TiXmlElement* rule_el = prod_el->FirstChildElement("Rule");
				rule_el != nullptr;
				rule_el = rule_el->NextSiblingElement("Rule")) {
			RuleT r(rule_el);
			this->add_rule(r);
		}
	}

	std::string create_variable(const std::string& name) {
		assert(!this->is_variable(name));
		this->V.insert(name);
		return name;
	}

	TiXmlElement* to_xml() {
		assert(this->check_rule_correctness());
		//TiXmlDocument doc;
		//TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
		TiXmlElement* root = new TiXmlElement("CFG");
		
		TiXmlElement* vars = new TiXmlElement("Variables");
		std::string varstring;
		for (std::string s: this->V) varstring += s;
		TiXmlText* vartext = new TiXmlText(varstring);
		vars->LinkEndChild(vartext);
		
		TiXmlElement* startsymbol = new TiXmlElement("StartSymbol");
		TiXmlText* symboltext = new TiXmlText(this->S);
		startsymbol->LinkEndChild(symboltext);
		vars->LinkEndChild(startsymbol);
		
		root->LinkEndChild(vars);
		
		TiXmlElement* terminals = new TiXmlElement("Terminals");
		std::string termstring;
		for (std::string s: this->T) termstring += s;
		TiXmlText* termtext = new TiXmlText(termstring);
		terminals->LinkEndChild(termtext);
		
		root->LinkEndChild(terminals);
		
		TiXmlElement* productions = new TiXmlElement("Productions");
		for (auto iter: this->P) {
			TiXmlElement* rule_el = this->get_rule(iter.second).to_xml();
			productions->LinkEndChild(rule_el);
		}
		
		root->LinkEndChild(productions);
		
		return root;
	}

};


using s_CFG = xml_CFG<SimpleRule>;

