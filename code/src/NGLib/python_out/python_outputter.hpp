
#pragma once

#include <string>

#include "Python.h"
#include "boost/python.hpp"

#include "NGLib/outputter/outputter.hpp"

#include "libs/tinyxml/tinyxml.h"


class PythonOutputter: public Outputter {
public:
	PythonOutputter(std::string _filename="output.py");
	
	PythonOutputter(TiXmlElement* root);
	
	void init();
	
	void output(std::string s);
	
	void close();
	
	TiXmlElement* to_xml();
	
private:
	std::string filename;
	boost::python::object main_module;
	boost::python::object main_namespace;
	boost::python::object func;
	
	bool has_close = false;
	static bool python_initialized;
};

