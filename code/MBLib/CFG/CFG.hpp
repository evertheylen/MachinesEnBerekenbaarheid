
/* CFG library

[bake me]

dependencies["objects"] = [
	"libs/tinyxml>>headers",
	"MBLib/common>>headers"
]

[stop baking]

*/

#ifndef H_CFG
#define H_CFG

#include <set>
#include <iostream>
#include <string>
#include <map>

#include <cassert>

#include "libs/tinyxml/tinyxml.h"
#include "MBLib/common/common.hpp"

template <typename ID_T, typename VarT, typename TermT>
class BaseCFG {
public:
	typedef std::basic_string<TermT> Production;
	typedef std::set<Production> ProductionSet;
	
	virtual bool has_names() = 0;
	
	// V ////////
	virtual SetBase<ID_T, VarT>* Variables() = 0;
	
	// If removing a variable, we should also remove their productions
	// Please do not use Variables()->remove(var), the CFG won't know it's variable
	// went missing
	void remove_var(ID_T var) {
		this->Variables()->remove(var);
		productions.erase(var);
	}
	
	// T ////////
	virtual SetBase<ID_T, TermT>* Terminals() = 0;
	
	// P ////////
	
	std::map<ID_T, ProductionSet>& get_productions() {
		return productions;
	}
	
	ProductionSet get_production(ID_T variable) {
		ProductionSet s;
		return s;
	}
	
	void add_production(ID_T variable, Production prod) {
		auto ps = productions.find(variable);
		if (ps == productions.end()) {
			// this variable has no productions yet
			productions[variable] = {prod};
		} else {
			// this variable already has productions
			ps.second.insert(prod);
		}
	}
	
	void set_productions(ID_T variable, ProductionSet prod_set) {
		productions[variable] = prod_set;
	}
	
	// S ////////
	ID_T get_start_symbol() {
		return S;
	}
	
	void set_start_symbol(ID_T newS) {
		assert(this->Variables()->contains(newS));
		S = newS;
	}
	
private:
	ID_T S;
	std::map<ID_T, ProductionSet> productions;
};


template <typename ID_T, typename VarT, typename TermT>
class NamedCFG: public BaseCFG<ID_T, VarT, TermT> {
public:
	typedef std::basic_string<TermT> Production;
	typedef std::set<Production> ProductionSet;
	
	bool has_names() { return true; }
	
	NamedCFG(VarT default_varname, std::set<VarT> _vars, TermT default_termname, std::set<TermT> _terms, ID_T _S=0):
			vars(default_varname, _vars), terms(default_termname, _terms) {
		this->set_start_symbol(_S);
	}
	
	NamedCFG(VarT default_varname, TermT default_termname, ID_T _S=0):
			vars(default_varname, {}), terms(default_termname, {}) {
		vars.add(_S);
		this->set_start_symbol(_S);
	}
	
	// V ////////
	SetBase<ID_T, VarT>* Variables() {
		return &vars;
	}
	
	// T ////////
	SetBase<ID_T, TermT>* Terminals() {
		return &terms;
	}
	
	// P and S are implemented in parent class
	
private:
	NamedSet<ID_T, VarT> vars;
	NamedSet<ID_T, TermT> terms;
};


template <typename ID_T, typename VarT, typename TermT>
class UnnamedCFG: public BaseCFG<ID_T, VarT, TermT> {
public:
	typedef std::basic_string<TermT> Production;
	typedef std::set<Production> ProductionSet;
	
	bool has_names() { return false; }
	
	UnnamedCFG(VarT default_varname, ID_T varsize, TermT default_termname, ID_T termsize, ID_T _S):
			vars(default_varname, varsize), terms(default_termname, termsize) {
		this->set_start_symbol(_S);
	}
	
	// V ////////
	SetBase<ID_T, VarT>* Variables() {
		return &vars;
	}
	
	// T ////////
	SetBase<ID_T, TermT>* Terminals() {
		return &terms;
	}
	
	// P and S are implemented in parent class
	
private:
	UnnamedSet<ID_T, VarT> vars;
	UnnamedSet<ID_T, TermT> terms;
};


typedef NamedCFG<unsigned int, std::string, std::string> s_CFG;
typedef UnnamedCFG<unsigned int, std::string, std::string> s_UnnamedCFG;





class CFG {
public:
	std::set<char> V;				// Variables
	std::set<char> T;  				// Terminals
	std::map<char, std::set<std::string>> P;	// Productions
	char S;							// Start symbol, \in V
	
	static constexpr const char* EPSILON = "$";

	CFG();
	CFG(TiXmlDocument& doc);
	
	friend std::ostream& operator<< (std::ostream& out, CFG& G);
	
	void to_xml(std::string filename);
};


#endif
