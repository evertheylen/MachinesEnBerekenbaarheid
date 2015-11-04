
#ifndef _REPLACOR_H
#define _REPLACOR_H

#include <string>
#include <vector>
#include <list>

class Replacor {
public:
	// TODO constructor with CFG?
	
	// a list so popping and pushing is easy
	// TODO: string refs look hackerish and error-prone
	virtual std::vector<std::string> replace(std::string var, std::list<std::string>& context) = 0;
	
	virtual bool replaceable(std::string symb) = 0;
	// in a CFG bounded class, this would be `return is_var(var)`
	// this function decides whether a symbol will be replaced or outputted
	
};

// Dummy replacor
class DumbReplacor: public Replacor {
public:
	
	std::vector<std::string> replace(std::string var, std::list<std::string>& context);
	
	bool replaceable(std::string symb);
};

#endif
