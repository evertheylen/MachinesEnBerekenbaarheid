
#ifndef H_COMMON
#define H_COMMON

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>


// Easy set union
template <typename T>
std::set<T> Union(std::set<T> A, std::set<T> B) {
	std::set<T> dest = A;
	dest.insert(B.begin(), B.end());
	return dest;
}

// split string in vector
std::vector<std::string> split(const std::string& s);


template <typename T>
std::string to_string(T input) {
	return std::to_string(input);
}

// Concatenate strings
template <>
std::string to_string<std::vector<std::string>>(std::vector<std::string> input);

template <>
std::string to_string<std::set<std::string>>(std::set<std::string> input);

template <>
std::string to_string<std::string>(std::string input);


// Output
template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& s) {
	for (const T& el: s) {
		out << el;
	}
	return out;
}


template <typename T>
std::ostream& operator<< (std::ostream& out, const std::set<T>& s) {
	out << "{";
	for (const T& el: s) {
		out << el << ", ";
	}
	out << "}";
	return out;
}


#endif

