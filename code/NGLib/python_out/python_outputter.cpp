
#include "python_outputter.hpp"

using namespace boost::python;

PythonOutputter::PythonOutputter(std::string _filename):
		filename(_filename) {}


void PythonOutputter::init() {
	// note that Python is written in C
	// forgive its ugliness
	Py_Initialize();
	
	// Boost.Python does a great job hiding all that though
	main_module = import("__main__");
	main_namespace = main_module.attr("__dict__");
	
	exec_file(filename.c_str(), main_namespace);
	exec("MyOutputter = Outputter()", main_namespace);
	
	object has_close_obj = eval("hasattr(MyOutputter, 'close')", main_namespace);
	has_close = extract<bool>(has_close_obj);
}


void PythonOutputter::output(std::string s) {
	std::string call = std::string("MyOutputter.output('''") + s + "''')";
	exec(call.c_str(), main_namespace);
}


void PythonOutputter::close() {
	if (has_close) {
		exec("MyOutputter.close()", main_namespace);
	}
	
	// exit PyInterpreter?
}
