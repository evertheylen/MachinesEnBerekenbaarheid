
#pragma once

#include <string>

#include "boost/python.hpp"
#include "Python.h"

#include "NGLib/outputter/outputter.hpp"
#include "../../libs/tinyxml/tinyxml.h"

class PythonOutputter: public Outputter {
public:
	PythonOutputter(std::string _filename="output.py");
	
	void init();
	
	void output(std::string s);
	
	void close();
	
	TiXmlElement* to_xml();
	
private:
	std::string filename;
	boost::python::object main_module;
	boost::python::object main_namespace;
	
	bool has_close = false;
};
