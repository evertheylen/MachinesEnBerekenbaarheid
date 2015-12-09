
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
		
	}
	
	ContextRule(const ID_T& _head, const std::vector<ID_T>& _body, NumT _num):
			SimpleRule<ID_T>(_head, _body, _num) {
		
	}
	
	TiXmlElement* to_xml() {
		TiXmlElement* root = SimpleRule<ID_T>::to_xml();
		
		return root;
	}
	
	
};

