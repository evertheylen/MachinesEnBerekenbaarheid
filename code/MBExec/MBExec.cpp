
/* [bake me]

executable = "MBExec"

dependencies["build_exec"] = [
	"MBLib>>build_objects",
	"libs/tinyxml>>build_objects"
]

[stop baking] */


#include <iostream>

#include "MBLib/CFG.hpp"
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
	} else {
		std::cout << "We don't know that action, sorry.\n";
	}
}
