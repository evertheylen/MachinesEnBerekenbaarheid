
#pragma once

#include "MBLib/CFG/CFG.hpp"
#include "MBLib/common/common.hpp"

#include <vector>
#include <map>
#include <set>
#include <iostream>

template <typename CFG_Type>
bool check_CNF(const CFG_Type& G, bool print=false) {
	bool is_CNF = true;  // until proven otherwise
	for (auto iter: G.P) {
		typename CFG_Type::ID_Type const& head = iter.first;
		for (const auto& body: iter.second) {
			if ( not ((body.size() == 2 and G.is_variable(body[0]) and G.is_variable(body[1])) or (body.size() == 1 and G.is_terminal(body[0])))) {
				if (print) {
					std::cout << "Violates CNF: " << head << " --> " << body << "\n";
					is_CNF = false;
				} else {
					return false;
				}
			}
		}
	}
	return is_CNF;
}

template <typename CFG_Type>
class Table {
public:
	typedef std::vector<std::set<typename CFG_Type::ID_Type>> DataT;
	DataT data;
	unsigned int inputsize;
	
	Table(unsigned int _inputsize): 
			inputsize(_inputsize) {
		unsigned int N = ((inputsize+1)*(inputsize))/2;
		data = DataT(N);
	}
	
	/*
	 * table for inputsize 5:
	 * 
	 *  s ^ 
	 *    | 14
	 *    | 12 13
	 *    |  9 10 11
	 *    |  5  6  7  8
	 *    |  0  1  2  3  4
	 *  0 +----------------->
	 *    0                 i
	 * 
	 */
	
	unsigned int loc(unsigned int s, unsigned int i) const {
		return data.size() - (((inputsize-s)*(inputsize-s+1))/2) + i;
	}
	
	std::set<typename CFG_Type::ID_Type>& get(unsigned int s, unsigned int i) {
		return data.at(loc(s,i));
	}
	
	const std::set<typename CFG_Type::ID_Type>& cget(unsigned int s, unsigned int i) const {
		return data.at(loc(s,i));
	}
	
	void set(unsigned int s, unsigned int i, const std::set<typename CFG_Type::ID_Type>& newset) {
		data.at(loc(s,i)) = newset;
	}
};


void pad(const std::string& s, std::ostream& out, unsigned int length);


template <typename CFG_Type>
std::ostream& operator<<(std::ostream& out, Table<CFG_Type>& t) {
	std::vector<std::vector<std::string>> output(t.inputsize);
	for (int s=t.inputsize-1; s>=0; s--) {
		for (unsigned int i=0; i<t.inputsize-s; i++) {
			//unsigned int j = i+s;
			output.at(s).push_back(to_string(t.cget(s,i)));
		}
	}
	
	std::vector<int> row_width(t.inputsize, 0);
	for (int s=t.inputsize-1; s>=0; s--) {
		for (unsigned int i=0; i<t.inputsize-s; i++) {
			if (output.at(s).at(i).size() > row_width.at(i)) {
				row_width.at(i) = output.at(s).at(i).size();
			}
		}
	}
	
	for (int s=t.inputsize-1; s>=0; s--) {
		out << " | ";
		for (unsigned int i=0; i<t.inputsize-s; i++) {
			pad(output.at(s).at(i), out, row_width.at(i));
			out << "  ";
		}
		out << "\n";
	}
	
	out << " +--------------------------------------------------------------------\n\n";
	return out;
}


template <typename CFG_Type>
bool CYK(const CFG_Type& G, typename CFG_Type::String& input, bool print=false) {
	assert(check_CNF(G,print));
	
	Table<CFG_Type> table(input.size());
	
	// build inverse of P
	std::map<typename CFG_Type::String, std::set<typename CFG_Type::ID_Type>> inv_P;
	for (auto iter: G.P) {
		typename CFG_Type::ID_Type const& head = iter.first;
		for (const auto& body: iter.second) {
			auto f = inv_P.find(body);
			if (f == inv_P.end()) {
				inv_P[body] = {head};
			} else {
				inv_P[body].insert(head);
			}
		}
	}
	
	// lowest row
	for (int i=0; i<input.size(); i++) {
		auto c = input[i];
		table.set(0, i, inv_P[{c}]); // one up for C++: this works (is correctly recognized as vector):D
	}
	
	if (print) {
		std::cout << "Initial table\n";
		std::cout << table;
	}
	
	// the rest!
	// working bottom-up ...
	for (int s=1; s<input.size(); s++) {
		// from left to right ...
		for (int i=0; i<input.size()-s; i++) {
			// this cell is ...
			int j = s+i;
			auto& cell = table.get(s,i);
			for (int k=i; k<j; k++) {
				// for each B in the cell below
				for (auto B: table.get(k-i, i)) {
					// for each C in the cell on the diagonal below right
					for (auto C: table.get(j-k-1, k+1)) {
						// the set has now been found!
						// just insert them all
						auto& the_set = inv_P[{B, C}];
						cell.insert(the_set.begin(), the_set.end());
					}
				}
			}
		}
		
		if (print) std::cout << table;
	}
	
	auto top = table.get(input.size()-1, 0);
	auto f = top.find(G.S);
	return f != top.end();
}

