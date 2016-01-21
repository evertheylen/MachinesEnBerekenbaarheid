
#pragma once

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <random>
#include <memory>

#include "libs/tinyxml/tinyxml.h"

#include "MBLib/new_CFG/CFG.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/python_out/python_outputter.hpp"
#include "NGLib/replacor/stochastic_replacor.hpp"
#include "NGLib/replacor/normal_replacor.hpp"
#include "NGLib/replacor/context_replacor.hpp"
#include "NGLib/exceptions/exceptions.hpp"


class Generator {
public:
	Generator();
	
	Generator(Replacor* _repl, Outputter* _out);
	
	Generator(const std::string& inputfile);
	
	// if max_repl == -1 ==> Infinite
	void generate(std::vector<std::string> start, int max_repl = -1);

	// this function will save to an XML file
	void saveXML(std::string filename);
	
	void setOutputter(Outputter* _out);
	
	Replacor* get_replacor() const;
	
	Outputter* get_outputter() const;
	
private:
	void rec_generate(std::string s, std::list<unsigned int>& context, int max_repl);
	
	std::unique_ptr<Outputter> out;
	std::unique_ptr<Replacor> repl;
};

