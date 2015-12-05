
#pragma once

#include <string>

#include "boost/python.hpp"
#include "Python.h"

#include "NGLib/outputter/outputter.hpp"

#ifndef __APPLE__

class PythonOutputter: public Outputter {
public:
	PythonOutputter(std::string _filename="output.py");
	
	void init();
	
	void output(std::string s);
	
	void close();
	
private:
	std::string filename;
	boost::python::object main_module;
	boost::python::object main_namespace;
	
	bool has_close = false;
};

#else

#include "NGLib/outputter/outputter.hpp"

using PythonOutputter = FileOutputter;

#endif
