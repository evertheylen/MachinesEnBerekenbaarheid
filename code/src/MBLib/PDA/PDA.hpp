

#ifndef H_PDA
#define H_PDA

#include <iostream>
#include <vector>
#include <set>
#include <map>

template <typename StateT, typename GammaT>
class deltaResultElement {
public:
	StateT new_state;
	std::vector<GammaT> replacement_stack;
	
	deltaResultElement(StateT n, std::vector<GammaT> repl):
			new_state(n), replacement_stack(repl) {}
	
	bool operator< (const deltaResultElement<StateT, GammaT>& other) const {
		if (new_state < other.new_state) {
			return true;
		} else {
			return this->replacement_stack < other.replacement_stack;
		}
	}
};


template <typename StateT, typename GammaT>
std::ostream& operator<< (std::ostream& out, deltaResultElement<StateT, GammaT> result) {
	out << "(" << result.new_state << ", " << result.replacement_stack << ")";
	return out;
}


template <typename StateT, typename SymbolT>
class PDA {
public:
	// there should be a way to represent epsilon in both Sigma and Gamma
	// therefore, they should be of the same type
	
	typedef StateT StateType;
	typedef SymbolT SymbolType;
	
	// PDA P = (Q, Sigma, Gamma, delta, q_0, Z_0, F)
	
	std::set<StateT> Q;
	
	std::set<SymbolT> Sigma;
	
	std::set<SymbolT> Gamma;
	
	// delta is slightly more complex
	typedef std::set<deltaResultElement<StateT, SymbolT>> deltaResult;   // deltaResult (new_state, replacement_stack)
														//   ^
	typedef std::map<SymbolT, deltaResult> deltaThird;	// stack symbol 
	typedef std::map<SymbolT, deltaThird> deltaSecond;	// input symbol
	typedef std::map<StateT, deltaSecond> deltaType;	// current state
	
	deltaType delta;
	
	StateT q0;
	
	SymbolT Z0;
	
	std::set<StateT> F;
	
	PDA(std::set<StateT> _Q, std::set<SymbolT> _Sigma, std::set<SymbolT> _Gamma, StateT _q0, SymbolT _Z0, std::set<StateT> _F):
			Q(_Q), Sigma(_Sigma), Gamma(_Gamma), q0(_q0), Z0(_Z0), F(_F) {}
	
	PDA() {}
	
	
	void add_delta(StateT q, SymbolT input, SymbolT stacksymbol, StateT result_newstate, std::vector<SymbolT> result_repl_stack) {
		deltaResultElement<StateT, SymbolT> result(result_newstate, result_repl_stack);
		
		auto f1 = delta.find(q);
		if (f1 != delta.end()) {
			auto f2 = f1->second.find(input);
			if (f2 != f1->second.end()) {
				auto f3 = f2->second.find(stacksymbol);
				if (f3 != f2->second.end()) {
					f3->second.insert(result);
				} else {
					f2->second[stacksymbol] = deltaResult({result});
				}
			} else {
				f1->second[input] = deltaThird();
				f1->second[input][stacksymbol] = deltaResult({result});
			}
		} else {
			delta[q] = deltaSecond();
			delta[q][input] = deltaThird();
			delta[q][input][stacksymbol] = deltaResult({result});
		}
	}
	
	deltaResult get_delta(StateT q, SymbolT input, SymbolT stacksymbol) {
		auto f1 = delta.find(q);
		if (f1 != delta.end()) {
			auto f2 = f1->second.find(input);
			if (f2 != f1->second.end()) {
				auto f3 = f2->second.find(stacksymbol);
				if (f3 != f2->second.end()) {
					return f3->second;
				}
			}
		}
		return deltaResult();  // empty
	}
};

typedef PDA<std::string, std::string> s_PDA;


template <typename StateT, typename SymbolT>
std::ostream& operator<< (std::ostream& out, PDA<StateT, SymbolT>& P) {
	out << "Q = " << P.Q << "\n";
	out << "Sigma = " << P.Sigma << "\n";
	out << "Gamma = " << P.Gamma << "\n";
	out << "delta = {\n";
	for (auto iter1: P.delta) {
		// iter1 = (state, deltaSecond)
		for (auto iter2: iter1.second) {
			// iter2 = (input_symbol, deltaThird)
			for (auto iter3: iter2.second) {
				// iter3 = (stack_symbol, deltaResult)
				out << "    (" << iter1.first << ", " << iter2.first << ", " << iter3.first << ") --> ";
				out << iter3.second << "\n";
			}
		}
	}
	
	out << "}\nq0 = " << P.q0 << "\n";
	out << "Z0 = " << P.Z0 << "\n";
	out << "F = " << P.F << "\n";
	return out;
}

#endif // H_PDA
