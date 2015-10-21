
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
	
	std::set<ID_T> V;										// Variables
	std::set<ID_T> T;  										// Terminals
	std::map<ID_T, std::set<std::vector<ID_T>>> P;	// Productions
	ID_T S;													// Start symbol, \in V
	
	static constexpr const char* EPSILON = "$";
	
	CFG() {}
	
	CFG(std::set<ID_T> _V, std::set<ID_T> _T, ID_T _S):
			V(_V), T(_T), S(_S) {}
	
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
	
	// Hi my name is Bjarne Stroustrup and fuck you 'cause there is no easy mechanism for inheriting constructors
	
	s_CFG();
	
	s_CFG(std::set<std::string> _V, std::set<std::string> _T, std::string _S);
	
	s_CFG(TiXmlDocument& doc);
	
	void to_xml(std::string filename);
};

#endif

