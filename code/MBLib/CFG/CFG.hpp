
/* CFG library

[bake me]

dependencies["objects"] = [
	"libs/tinyxml>>headers"
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
