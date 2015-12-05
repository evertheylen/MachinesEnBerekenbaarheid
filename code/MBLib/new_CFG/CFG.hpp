
/* CFG library

[bake me]

dependencies["headers"] = [
	"libs/tinyxml>>headers",
	"MBLib/common>>headers",
	"NGLib/exceptions>>headers"
]

# BUG in Baker :'(
dependencies["build_test_exec"] = [
	"libs/tinyxml>>build_objects",
	"MBLib/common>>build_objects"
]

[stop baking]

*/


#ifndef H_CFG
#define H_CFG

#include <set>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <cassert>

#include "libs/tinyxml/tinyxml.h"
#include "MBLib/common/common.hpp"
#include "NGLib/exceptions/exceptions.hpp"

template <typename ID_T>
class SimpleRule {
public:
	using ID_Type = ID_T;
	using StringT = std::vector<ID_T>;
	
	SimpleRule(TiXmlElement* root) {
		head = std::string(root->Attribute("LHS"));
		body = split(std::string(root->Attribute("RHS")));
	}
	
	SimpleRule(const ID_T& _head, const std::vector<ID_T>& _body):
		head(_head), body(_body) {}
	
	const ID_T& get_head() const {
		return head;
	}
	
	const std::vector<ID_T>& get_body() const {
		return body;
	}
	
	TiXmlElement* to_xml() {
		TiXmlElement* rule_el = new TiXmlElement("Rule");
		rule_el->SetAttribute("LHS", head);
		rule_el->SetAttribute("RHS", to_string(body));
		return rule_el;
	}
	
	ID_T head;
	std::vector<ID_T> body;
};


template <typename RuleT>
class RuleIterator {
public:
	using ID_T = typename RuleT::ID_Type;
	using MapT = typename std::multimap<ID_T, RuleT>;
	
	RuleIterator(const ID_T& _var, const MapT& _P):
			var(_var), P(_P) {}
	
	typename MapT::const_iterator begin() {
		return P.find(var);
	}
	
	typename MapT::const_iterator end() {
		return P.end();
	}
	
	unsigned int size() {
		return P.count(var);
	}
	
private:
	const ID_T& var;
	const MapT& P;
};


// Description of what RuleT should offer:
//   - ID_Type type declaration
//   - String type declaration
//   - const ID_Type& get_head() method
template <typename RuleT=SimpleRule<std::string>>
class CFG {
public:
	using Rule_Type = RuleT;
	using ID_Type = typename RuleT::ID_Type;
	using ID_T = ID_Type;  // legacy support :P
	using StringT = typename RuleT::StringT;
	
	
	std::set<ID_T> V;						// Variables
	std::set<ID_T> T;  						// Terminals
	std::multimap<ID_T, RuleT> P;			// Productions
	ID_T S;									// Start symbol, \in V
	
	CFG() {}
	
	CFG(std::set<ID_T> _V, std::set<ID_T> _T, ID_T _S):
			V(_V), T(_T), S(_S) {}
	
	bool is_epsilon(StringT s) const {
		return s.empty();
	}
	
	bool is_variable(const ID_T& symbol) const {
		auto m = V.find(symbol);
		return m != V.end();
	}
	
	bool is_terminal(const ID_T& symbol) const {
		auto m = T.find(symbol);
		return m != T.end();
	}
	
	bool has_rules(const ID_T& var) const {
        return P.find(var) != P.end();
	}
	
	RuleIterator<RuleT> rules(const ID_T& var) const {
		return RuleIterator<RuleT>(var, P);
	}
	
	void add_rule(const RuleT& rule) {
		assert(is_variable(rule.get_head()));
		auto m = P.find(rule.get_head());
		if (m == P.end()) {
			P[rule.get_head()] = {rule};
		} else {
			m->second.insert(rule);
		}
		assert(check_rule_correctness());
	}
	
	// assert this as much as you want
	bool check_rule_correctness() {
		for (auto& i: P) {
			if (i.first != i.second.get_head()) {
				return false;
			}
		}
		return true;
	}
	
};


template <typename ID_T>
std::ostream& operator<< (std::ostream& out, CFG<ID_T>& G) {
	out << "Variables = " << G.V << "\n";
	out << "Terminals = " << G.T << "\n";
	
	out << "Productions = {\n";
	for (auto iter: G.P) {
		for (auto rule: iter.second) {
			out << "  " << to_string(iter.first) << " --> " << to_string(rule) << "\n";
		}
	}
	
	out << "}\nStart symbol = " << G.S << "\n";
	return out;
}


// xml_CFG requires more for RuleT:
//   - constructor RuleT(head, body)
//   - std::vector<ID_Type>& get_body() method
//   - string based (Basically, ID_Type should be std::string)
template <typename RuleT>
class xml_CFG: public CFG<RuleT> {
public:
	
	using Rule_Type = RuleT;
	using ID_Type = typename RuleT::ID_Type;
	using ID_T = ID_Type;
	
	using Parent = CFG<RuleT>;
	
	// Hi my name is Bjarne Stroustrup and fuck you 'cause there is no easy mechanism for inheriting constructors
	// Did I tell you about typedefs? No easy way to inherit them either :D Have fun with C++! lol
	
	xml_CFG() {}

	xml_CFG(std::set<ID_T> _V, std::set<ID_T> _T, ID_T _S):
			Parent(_V, _T, _S) {}

	xml_CFG(TiXmlElement* root) {
		auto vars_el = root->FirstChildElement("Variables");
		if (vars_el == nullptr) throw syntacticError();
		for (auto s: split(std::string(vars_el->GetText()))) {
			this->V.insert(s);
		}
		this->S = std::string(vars_el->FirstChildElement("StartSymbol")->GetText());
		
		auto term_el = root->FirstChildElement("Terminals");
		if (term_el == nullptr) throw syntacticError();
		for (auto s: split(std::string(term_el->GetText()))) {
			this->T.insert(s);
		}
		
		auto prod_el = root->FirstChildElement("Productions");
		if (prod_el == nullptr) throw syntacticError();
		for (TiXmlElement* rule_el = prod_el->FirstChildElement("Rule");
				rule_el != nullptr;
				rule_el = rule_el->NextSiblingElement("Rule")) {
			RuleT r = RuleT(rule_el);
			this->P.insert(std::make_pair(r.get_head(), r));
		}
	}

	std::string create_variable(const std::string& name) {
		assert(!this->is_variable(name)); // TODO propose other alternative instead of failing
		this->V.insert(name);
		return name;
	}

	TiXmlElement* to_xml(/*std::string filename*/) {
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
		for (auto rule: this->P) {
			TiXmlElement* rule_el = rule.second.to_xml();
			productions->LinkEndChild(rule_el);
		}
		
		root->LinkEndChild(productions);
		
		return root;
	}

};


using s_CFG = xml_CFG<SimpleRule<std::string>>;

#endif

