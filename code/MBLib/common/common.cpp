
#include "common.hpp"

// split string in vector
std::vector<std::string> split(const std::string& s) {
	std::vector<std::string> result;
	std::string buf;
	for (char c: s) {
		if (buf.size() == 0) {
			if (c == '[') {
				buf += '[';
			} else {
				result.push_back(std::string(1,c));
			}
		} else {
			if (c == ']') {
				buf += ']';
				result.push_back(buf);
				buf.clear();
			} else {
				buf += c;
			}
		}
	}
	return result;
}

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

