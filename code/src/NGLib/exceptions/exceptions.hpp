
#pragma once

#include <exception>
#include <string>

class NGException: public std::exception {
public:
	NGException(const std::string& _info);
	
	const char* what() const throw();
	
private:
	std::string info;
};

class SyntacticError: public NGException {
public:
	SyntacticError(const std::string& _info);
};

class NoValidFilename: public NGException {
public:
	NoValidFilename(const std::string& _info);
};

class SemanticError: public NGException {
public:
	SemanticError(const std::string& fault, const std::string& correct);
};
