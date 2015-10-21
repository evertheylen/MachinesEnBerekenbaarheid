
#include "common.hpp"


template <>
std::string to_string<std::vector<std::string>>(std::vector<std::string> input) {
	std::string s;
	for (std::string& p: input) {
		s += p;
	}
	return s;
}

template <>
std::string to_string<std::string>(std::string input) {
	return input;
}

