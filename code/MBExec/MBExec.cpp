
/* [bake me]

executable = "MBExec"

dependencies["build_exec"] = [
    "MBLib/CNF>>build_objects",
	"MBLib/CFG>>build_objects",
	"MBLib/CNF>>build_objects",
	"libs/tinyxml>>build_objects"
]

[stop baking] */


#include <iostream>

#include "MBLib/CFG/CFG.hpp"
#include "MBLib/CNF/CNF.hpp"
#include "libs/tinyxml/tinyxml.h"

#define ENSURE_ARGCOUNT(k) if (argc < k) { std::cout << "Not enough arguments." << std::endl; return 1; }

int main(int argc, char** argv) {
	ENSURE_ARGCOUNT(2);
	
	std::string action = argv[1];
	if (action == "parse_CFG") {
		ENSURE_ARGCOUNT(3);
		TiXmlDocument doc(argv[2]);
		doc.LoadFile();
		s_CFG G(doc);
		std::cout << G;
	} else if (action == "write_CFG") {
		ENSURE_ARGCOUNT(4);
		TiXmlDocument doc(argv[2]);
		doc.LoadFile();
		s_CFG G(doc);
		G.to_xml(std::string(argv[3]));
	} 
	else if (action == "CNF") {
		ENSURE_ARGCOUNT(3);
		TiXmlDocument doc_cfg(argv[2]);
		doc_cfg.LoadFile();
		s_CFG cfg(doc_cfg);
		s_CFG cleaned_up_cfg = CNF(cfg); // Step one for cnf: clean up grammar.
		std::cout << cleaned_up_cfg << std::endl;
	} 
	else {
		std::cout << "We don't know that action, sorry.\n";
	}
}
