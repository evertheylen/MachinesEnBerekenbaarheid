#include "replacor.hpp"

// Dummy replacor
std::vector<std::string> DumbReplacor::replace(std::string var, std::list<std::string>& context) {
	return {"(", var, ")"};
}

bool DumbReplacor::replaceable(std::string symb) {
	if (symb == "(" or symb == ")") {
		return false;
	}
	return true;
}
