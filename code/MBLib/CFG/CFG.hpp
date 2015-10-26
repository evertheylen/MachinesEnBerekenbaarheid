
/* CFG library

[bake me]

dependencies["headers"] = [
	"libs/tinyxml>>headers",
	"MBLib/common>>headers",
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



template <typename ID_T>
class CFG {
public:
	typedef ID_T ID_Type;
	typedef std::vector<ID_T> String;
	
	std::set<ID_T> V;										// Variables
	std::set<ID_T> T;  										// Terminals
	std::map<ID_T, std::set<std::vector<ID_T>>> P;	// Productions
	ID_T S;													// Start symbol, \in V
	
	CFG() {}
	
	CFG(std::set<ID_T> _V, std::set<ID_T> _T, ID_T _S):
			V(_V), T(_T), S(_S) {}
	
	bool is_epsilon(String s) const {
		return s.empty();
	}
	
	bool is_variable(ID_T symbol) const {
		auto m = V.find(symbol);
		return m != V.end();
	}
	
	bool is_terminal(ID_T symbol) const {
		auto m = T.find(symbol);
		return m != T.end();
	}
	
	std::set<String> get_bodies(ID_T var) const {
		assert(is_variable(var));
		auto m = P.find(var);
		if (m == P.end()) {
			return std::set<String>();  // empty set
		} else {
			return m->second;
		}
	}
	
	void set_bodies(ID_T var, std::set<String> bodies) {
		assert(is_variable(var));
		P[var] = bodies;
	}
	
	void add_production(ID_T var, String new_body) {
		assert(is_variable(var));
		auto m = P.find(var);
		if (m == P.end()) {
			P[var] = {new_body};
		} else {
			m->second.insert(new_body);
		}
	}
	
	//template <typename _ID_T>
	//friend std::ostream& operator<< (std::ostream& out, CFG<ID_T>& G);
	
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



class s_CFG: public CFG<std::string> {
public:
	
	typedef std::string ID_Type;
	typedef std::vector<std::string> String;
	
	// Hi my name is Bjarne Stroustrup and fuck you 'cause there is no easy mechanism for inheriting constructors
	// Did I tell you about typedefs? No easy way to inherit them either :D Have fun with C++! lol
	
	s_CFG();
	
	s_CFG(std::set<std::string> _V, std::set<std::string> _T, std::string _S);
	
	s_CFG(TiXmlDocument& doc);
	
	std::string create_variable(const std::string& name);
	
	void to_xml(std::string filename);
};

#endif

