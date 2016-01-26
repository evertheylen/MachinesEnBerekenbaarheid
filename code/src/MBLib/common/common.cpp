
#include "common.hpp"

// split string in vector
std::vector<std::string> split(const std::string& s) {
	std::vector<std::string> result;
	std::string buf;
	bool escaped = false;
	for (char c: s) {
		if (c == '%') {
			if (escaped) {
				buf += '%';
				escaped = false;
			} else {
				escaped = true;
			}
		} else if (buf.size() == 0) {
			if (c == '[' and not escaped) {
				buf += '[';
			} else {
				buf += c;
				result.push_back(buf);
				buf.clear();
				escaped = false;
			}
		} else {
			if (c == ']' and not escaped) {
				buf += ']';
				result.push_back(buf);
				buf.clear();
			} else {
				buf += c;
				escaped = false;
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
std::string to_string<std::set<std::string>>(std::set<std::string> input) {
	std::string s = "{";
	for (const std::string& p: input) {
		s += p;
		s += ", ";
	}
	s += "}";
	return s;
}

template <>
std::string to_string<std::string>(std::string input) {
	return input;
}

