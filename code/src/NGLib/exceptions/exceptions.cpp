#include "exceptions.hpp"

NGException::NGException(const std::string& _info): info(_info) {}
	
const char* NGException::what() const throw() {
	return info.c_str();
}

SyntacticError::SyntacticError(const std::string& _info):
    NGException(std::string("SyntacticError: ") + _info) {}

NoValidFilename::NoValidFilename(const std::string& _info):
	NGException(_info + " isn't a valid filename") {}

SemanticError::SemanticError(const std::string& fault, const std::string& correct):
	NGException(std::string("Semantic error: ") + fault + " instead of " + correct + ".") {}
