
#pragma once

#include <exception>
#include <string>

class NGException: public std::exception {
public:
	NGException(const std::string& _info): info(_info) {}
	
	const char* what() const throw() {
		return info.c_str();
	}
	
private:
	std::string info;
};

class SyntacticError: public NGException {
public:
	SyntacticError(const std::string& _info):
		NGException(std::string("SyntacticError: ") + _info) {}
};

class NoValidFilename: public NGException {
public:
	NoValidFilename(const std::string& _info):
		NGException(_info + " isn't a valid filename") {}
};

class SemanticError: public NGException {
public:
	SemanticError(const std::string& fault, const std::string& correct):
		NGException(std::string("Semantic error: ") + fault + " instead of " + correct + ".") {}
};
