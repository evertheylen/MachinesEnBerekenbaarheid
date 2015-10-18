
/* [bake me]

executable = "MBExec"

dependencies["build_exec"] = [
	"@MBLib>>build_objects",
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
		CFG G(doc);
		std::cout << G;
	} else if (action == "write_CFG") {
		ENSURE_ARGCOUNT(4);
		TiXmlDocument doc(argv[2]);
		doc.LoadFile();
		CFG G(doc);
		G.to_xml(std::string(argv[3]));
	} else if (action == "CNF") {
		ENSURE_ARGCOUNT(3);
		TiXmlDocument doc_cfg(argv[2]);
		doc_cfg.LoadFile();
		CFG cfg(doc_cfg);
		CFG cleaned_up_cfg = eliminate_useless_symbols(eliminate_unit_pairs(eliminate_epsilon_productions(cfg))); // Step one for cnf: clean up grammar.
		std::cout << cleaned_up_cfg << std::endl;
	} else {
		std::cout << "We don't know that action, sorry.\n";
	}
}
