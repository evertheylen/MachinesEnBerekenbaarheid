
// bake_me is in different file

#include <memory>
#include <exception>

#ifndef __APPLE__
#include "boost/python.hpp"
#endif

#include "NGLib/generator/generator.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/python_out/python_outputter.hpp"
#include "NGLib/replacor/replacor.hpp"


int main(int argc, char** argv) {
	std::vector<std::string> args;
	for (int i=1; i<argc; i++) {
		args.push_back(argv[i]);
	}
	
#ifndef __APPLE__
	try {
#endif
		std::unique_ptr<GeneratorInterface> g(loadXML(args.size() < 1 ? "loadXML.xml" : args.at(0)));
		g->generate({"A", "BC"}, 10);
		g->saveXML("saveXML.xml");
#ifndef __APPLE__
	} catch (boost::python::error_already_set e) {
		std::cout << "Core crashed with Python Exception:\n";
		PyErr_Print();
	} 
#endif
// 	catch (std::exception& e) {
// 		std::cout << "Core crashed with std::exception.\n";
// 		std::cout << e.what();
// 		std::cout << "\n";
// 	} catch (...) {
// 		std::cout << "Core crashed with an unknown exception.\n";
// 	}
}
