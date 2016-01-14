
#pragma once

#include <set>
#include <iostream>
#include <string>
#include <map>
#include <vector>

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
		std::cout << "this is spartaaa\n";
// 		std::cout << "entering parse\n";
		for (TiXmlElement* el = root->FirstChildElement(); el != nullptr; el = el->NextSiblingElement()) {
			NumT p_num = std::stoi(el->Attribute("ID"));
			double chance = std::stod(el->Attribute("c"));
			table[p_num] = chance;
// 			std::cout << "setting chance of " << p_num << " to " << chance << "\n";
		}
	}
	
	ContextRule(const ID_T& _head, const std::vector<ID_T>& _body, NumT _num):
			SimpleRule<ID_T>(_head, _body, _num) {}
	
	TiXmlElement* to_xml() {
		TiXmlElement* root = SimpleRule<ID_T>::to_xml();
		for (auto it: table) {
			TiXmlElement* prob_el = new TiXmlElement("prob");
			prob_el->SetAttribute("ID", it.first);
			prob_el->SetAttribute("c", to_string(it.second));
			root->LinkEndChild(prob_el);
		}
		return root;
	} 
	
	std::map<NumT, double> table;
};

