/*
 * CNF.cpp
 *
 *  Created on: 17 Oct 2015
 *      Author: stijn
 */
#include "../CFG.hpp"
#include "CNF.hpp"
#include <set>
#include <iostream>

CFG CNF(const CFG& input_cfg) {

}

std::set<char> find_nullable_symbols(const CFG& input_cfg) {
	std::set<char> nullable_symbols;
	for (auto iter : input_cfg.P) {
		for (auto rule : iter.second) {
			if (rule == "$") { // TODO Find a decent symbol for epsilon, for the moment: $
				if (nullable_symbols.find(iter.first) == nullable_symbols.end()) nullable_symbols.insert(iter.first);
			}
		}
	}

	bool added_new_nullable = true;
	while(added_new_nullable) {
		added_new_nullable = false;
		for (auto iter : input_cfg.P) {
			for (auto rule : iter.second) {
				bool add = true;
				for (unsigned int i = 0; i < rule.length(); i++) {
					if (nullable_symbols.find(rule.at(i)) == nullable_symbols.end()) {
						add = false;
						break;
					}
				}
				if (add) {
					if (nullable_symbols.find(iter.first) == nullable_symbols.end()) {
						nullable_symbols.insert(iter.first);
						added_new_nullable = true;
					}
				}
			}
		}
	}
	for (auto symbol: nullable_symbols) {
		std::cout << "Nullable symbols " << symbol << std::endl;
	}
}

