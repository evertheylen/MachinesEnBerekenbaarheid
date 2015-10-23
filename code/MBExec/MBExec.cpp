
/* [bake me]

executable = "MBExec"

dependencies["build_exec"] = [
	"MBLib/CFG>>build_objects",
	"MBLib/CNF>>build_objects",
	"MBLib/CFG_PDA>>build_objects",
	"MBLib/PDA_CFG>>build_objects",
	"libs/tinyxml>>build_objects",
	"MBLib/PDA>>build_objects",
]


[stop baking] */


#include <iostream>

#include "MBLib/CFG/CFG.hpp"
#include "MBLib/CNF/CNF.hpp"
#include "MBLib/PDA/PDA.hpp"
#include "MBLib/CFG_PDA/CFG_PDA.hpp"
#include "MBLib/PDA_CFG/PDA_CFG.hpp"
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
		
	} else if (action == "CNF") {
		ENSURE_ARGCOUNT(3);
		TiXmlDocument doc_cfg(argv[2]);
		doc_cfg.LoadFile();
		s_CFG cfg(doc_cfg);
		s_CFG cnf_cfg = CNF(cfg); // Step one for cnf: clean up grammar.
		std::cout << cnf_cfg << std::endl;
		
	} else if (action == "parse_PDA") {
		PDA<std::string, std::string> P;
		P.Q = {"q0", "q1"};
		P.Sigma = {"a", "b"};
		P.Gamma = {"X", "Y", "Z"};
		P.Z0 = "X";
		P.q0 = "q0";
		P.add_delta("q0", "a", "X", "q1", {"X", "Y", "Z"});
		std::cout << P;
		
	} else if (action == "CFG_to_PDA") {
		ENSURE_ARGCOUNT(3);
		TiXmlDocument doc(argv[2]);
		doc.LoadFile();
		s_CFG G(doc);
		std::cout << G;
		std::cout << "\n=== Converted to PDA: ===\n\n";
		s_PDA P = CFG_to_PDA<s_CFG, s_PDA>(G);
		std::cout << P;
		
	} else if (action == "PDA_to_CFG") {
		PDA<std::string, std::string> P;
		P.Q = {"q0", "q1"};
		P.Sigma = {"a", "b"};
		P.Gamma = {"X", "Y", "Z"};
		P.Z0 = "X";
		P.q0 = "q0";
		P.add_delta("q0", "a", "X", "q1", {"X", "Y", "Z"});
		std::cout << P;
		std::cout << "\n=== Converted to CFG: ===\n\n";
		s_CFG G = PDA_to_CFG<s_PDA, s_CFG>(P);
		std::cout << G;
		
	} else if (action == "CFG_to_PDA_to_CFG") {
		ENSURE_ARGCOUNT(3);
		TiXmlDocument doc(argv[2]);
		doc.LoadFile();
		s_CFG G(doc);
		std::cout << G;
		std::cout << "\n=== Converted to PDA: ===\n\n";
		s_PDA P = CFG_to_PDA<s_CFG, s_PDA>(G);
		std::cout << P;
		std::cout << "\n=== Converted to CFG again: ===\n\n";
		s_CFG G2 = PDA_to_CFG<s_PDA, s_CFG>(P);
		std::cout << G2;
		
	} else {
		std::cout << "We don't know that action, sorry.\n";
	}
}
