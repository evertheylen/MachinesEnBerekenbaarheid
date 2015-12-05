
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


int main() {
	try {
		std::unique_ptr<GeneratorInterface> g(loadXML("loadXML.xml"));
		g->generate({"A", "BC"}, 10);
#ifndef __APPLE__
	} catch (boost::python::error_already_set e) {
		std::cout << "Core crashed with Python Exception:\n";
		PyErr_Print();
#endif
	} 
// 	catch (std::exception& e) {
// 		std::cout << "Core crashed with std::exception.\n";
// 		std::cout << e.what();
// 		std::cout << "\n";
// 	} catch (...) {
// 		std::cout << "Core crashed with an unknown exception.\n";
// 	}
}
