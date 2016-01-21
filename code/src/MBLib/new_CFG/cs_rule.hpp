
#pragma once

#include <set>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <sstream> 

#include <cassert>

#include "libs/tinyxml/tinyxml.h"
#include "MBLib/common/common.hpp"
#include "MBLib/new_CFG/CFG.hpp"


template <typename ID_T>
class ContextRule: public SimpleRule<ID_T> {
public:
	using ID_Type = ID_T;
	using StringT = std::vector<ID_T>;
	using NumT = unsigned int;
	
	ContextRule() = default;
	
	ContextRule(TiXmlElement* root): SimpleRule<ID_T>(root) {
// 		std::cout << "entering parse\n";
		for (TiXmlElement* el = root->FirstChildElement(); el != nullptr; el = el->NextSiblingElement()) {
			const char* num_attr = el->Attribute("ID");
			if (num_attr == nullptr) throw SyntacticError("No ID attribute");
			const char * p_num_attr = el->Attribute("c");
			if (p_num_attr == nullptr) throw SyntacticError("No c attribute");
			
			NumT p_num = std::stoi(num_attr);
			double chance = std::stod(p_num_attr);
			table[p_num] = chance;
// 			std::cout << "setting chance of " << p_num << " to " << chance << "\n";
		}
	}
	
	void fill_table(unsigned int i) {
		if (table.find(i) == table.end()) {
			table[i] = 1.0;
		}
	}
	
	ContextRule(const ID_T& _head, const std::vector<ID_T>& _body, NumT _num):
			SimpleRule<ID_T>(_head, _body, _num) {}
	
	TiXmlElement* to_xml() {
		TiXmlElement* root = SimpleRule<ID_T>::to_xml();
		for (auto it: table) {
			TiXmlElement* prob_el = new TiXmlElement("prob");
			prob_el->SetAttribute("ID", it.first);
			// more precision for chance
			std::stringstream morePrecision;
			morePrecision << it.second;
			prob_el->SetAttribute("c", morePrecision.str());
			root->LinkEndChild(prob_el);
		}
		return root;
	}
	
	std::map<NumT, double> table;
};

