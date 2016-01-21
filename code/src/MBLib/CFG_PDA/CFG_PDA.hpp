
#pragma once

#include "MBLib/CFG/CFG.hpp"
#include "MBLib/PDA/PDA.hpp"

// currently only works for strings
template <typename CFG_T, typename PDA_T>
PDA_T CFG_to_PDA(CFG_T& G) {
	PDA_T P({"q"}, G.T, Union(G.V, G.T), "q", G.S, {});
	
	// that was easy, now delta
	
	// for every variable (having productions)
	for (auto iter: G.P) {
		// for every body in a production of such a variable
		for (auto body: iter.second) {
			P.add_delta("q", "[eps]", iter.first, "q", body);
		}
	}
	
	// for every terminal
	for (auto term: G.T) {
		P.add_delta("q", term, term, "q", {"[eps]"});
	}
	
	return P;
}

