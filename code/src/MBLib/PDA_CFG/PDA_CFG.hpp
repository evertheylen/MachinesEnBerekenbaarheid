
#ifndef H_PDA_CFG
#define H_PDA_CFG

#include "MBLib/common/common.hpp"
#include "MBLib/CFG/CFG.hpp"
#include "MBLib/PDA/PDA.hpp"

#include <cassert>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

class SetChoice {
public:
	std::vector<unsigned int> choice;
	unsigned int max; // max is like 10 for decimal numbers, the highest you can go is 9 per digit
	
	SetChoice(unsigned int _max, int length);
	
	bool next_choice();
	
	unsigned int size();
	
	unsigned int at(unsigned int i);
	
	template <typename T>
	std::vector<T> pick(std::vector<T> base) {
		std::vector<T> result(size());
		for (int i=0; i<size(); i++) {
			result[i] = base.at(at(i));
		}
		return result;
	}
};




template <typename PDA_T, typename CFG_T>
CFG_T PDA_to_CFG(PDA_T& P) {
	CFG_T G({"S"}, P.Sigma, "S");
	
	std::vector<std::string> ordered_P_Q;
	for (auto q: P.Q) ordered_P_Q.push_back(q);
	
	// still two things to do: add production rules and add symbols to V when necessary
	// for a in P.Sigma + {eps}
	for (auto iter1: P.delta) {
		// iter1 = (state, deltaSecond)
		for (auto iter2: iter1.second) {
			// iter2 = (input_symbol, deltaThird)
			for (auto iter3: iter2.second) {
				// iter3 = (stack_symbol, deltaResult)
				const std::string& q = iter1.first;
				const std::string& a = iter2.first;
				const std::string& X = iter3.first;
				
				for (auto rule: iter3.second) {
					std::string& r0 = rule.new_state;
					std::vector<std::string>& Y = rule.replacement_stack;
					unsigned int k = Y.size();
// 					std::cout << "k is " << k << "\n";
					
					if (k == 0) {
						std::string head = std::string("[") + q + "/" + X + "/" + r0 + "]";
						G.V.insert(head);
						std::vector<std::string> body = {a};
						G.P[head].insert(body);
					} else if (k == 1) {
						for (auto r1: ordered_P_Q) {
							std::string head = std::string("[") + q + "/" + X + "/" + r1 + "]";
							G.V.insert(head);
							std::string first = std::string("[") + r0 + "/" + Y[0] + "/" + r1 + "]";
							G.V.insert(first);
							std::vector<std::string> body = {a, first};
						}
					} else {
						SetChoice c(ordered_P_Q.size(), std::max((int) k-1,0));
						do {
							auto other_r = c.pick(ordered_P_Q);
							
							std::string head = std::string("[") + q + "/" + X + "/" + other_r.at(k-2) + "]";
							G.V.insert(head);
							std::string first = std::string("[") + r0 + "/" + Y[0] + "/" + other_r.at(0) + "]";
							G.V.insert(first);
							std::vector<std::string> body = {a, first};
							for (int i=1; i<k-2; i++) {
								std::string var = std::string("[") + other_r[i] + "/" + Y.at(i+1) + "/" + other_r.at(i+1) + "]";
								G.V.insert(var);
								body.push_back(var);
							}
							G.P[head].insert(body);
						} while (c.next_choice());
					}
				}
			}
		}
	}
	
	return G;
}


#endif
