
#include "python_outputter.hpp"

#include <iostream>

using namespace boost::python;

PythonOutputter::PythonOutputter(std::string _filename):
		filename(_filename) {}

PythonOutputter::PythonOutputter(TiXmlElement* root) {
	filename = root->Attribute("filename");
}


void PythonOutputter::init() {
	// note that Python is written in C
	// forgive its ugliness
	Py_Initialize();
	
	wchar_t arg1[] = L"NextGen";
	wchar_t* argv[] = {arg1};
	PySys_SetArgv(1, argv);
	
// 	std::cout << "Initialized Python\n";
	
	// Boost.Python does a great job hiding all that though
	main_module = import("__main__");
	main_namespace = main_module.attr("__dict__");
	
// 	std::cout << "Initialized main stuff\n";
// 	std::cout << "filename = " << filename << "\n";
	
	exec_file(filename.c_str(), main_namespace);
	exec("MyOutputter = Outputter()", main_namespace);
	
// 	std::cout << "Init MyOutputter\n";
	
	object has_close_obj = eval("hasattr(MyOutputter, 'close')", main_namespace);
	has_close = extract<bool>(has_close_obj);
}


void PythonOutputter::output(std::string s) {
// 	std::cout << "Outputting " << s << "\n";
	object func = eval("MyOutputter.output", main_namespace);
	object my_outp = eval("MyOutputter", main_namespace);
	func(str(s.c_str()));
	//std::string call = std::string("MyOutputter.output('''") + s + "''')";
	//exec(call.c_str(), main_namespace);
}


void PythonOutputter::close() {
	if (has_close) {
		exec("MyOutputter.close()", main_namespace);
	}
	
	// exit PyInterpreter?
}

TiXmlElement* PythonOutputter::to_xml() {
	TiXmlElement* elem = new TiXmlElement("PYTHON_OUTPUTTER");
	elem->SetAttribute("filename", filename);
	return elem;
}

