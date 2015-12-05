
#pragma once

#include <exception>
#include <string>

class NGException: public std::exception {};

class syntacticError: public NGException {
public:
	virtual const char* what() const throw() {
		return "The input XML contains syntactic errors\n";
	}
};

class noValidFilename: public NGException {
public:
	virtual const char* what() const throw() {
		return "The file is not valid or not found\n";
	}
};

class semanticError: public NGException {
public:
	semanticError(const std::string& _fault, const std::string& _correct):
		fault(_fault), correct(_correct) {}
	
	virtual const char* what() const throw() {
		std::string error = "The inputXML contains semantic errors: " + fault + " instead of " + correct + ".\n";
		return error.c_str();
	}
private:
	std::string fault;
	std::string correct;
};
