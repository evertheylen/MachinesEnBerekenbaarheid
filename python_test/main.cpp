
#include <string>
#include <iostream>

#include "boost/python.hpp"
#include "Python.h"

using namespace boost::python;

int main(int argc, char** argv) {
	std::string filename("output.py");
	if (argc >= 2) {
		filename = std::string(argv[1]);
	}
	
	try {
		Py_Initialize();
		object main_module = import("__main__");
		object main_namespace = main_module.attr("__dict__");
		
		exec_file(filename.c_str(), main_namespace);
		
		exec("obj = SomeClass('Mario')", main_namespace);
		exec("result = obj.hello('Mr')", main_namespace);
		std::string name(extract<const char*>(main_namespace[str("result")]));
		std::cout << name << "\n";
		
	} catch (error_already_set) {
		PyErr_Print();
	}
}
