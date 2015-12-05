
// bake_me is in different file

#include <memory>
#include <exception>

#include "boost/python.hpp"

#include "NGLib/generator/generator.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/python_out/python_outputter.hpp"
#include "NGLib/replacor/replacor.hpp"


int main() {
	try {
		std::unique_ptr<PythonOutputter> fo(new PythonOutputter());
		std::unique_ptr<DumbReplacor> dr(new DumbReplacor);
		Generator<DumbReplacor> g(fo.get(), dr.get());
		g.generate({"A", "BC"}, 10);
		//g.loadXML("loadXML.xml");
		//g.saveXML("");
#ifndef __APPLE__
	} catch (boost::python::error_already_set e) {
		std::cout << "Core crashed with Python Exception:\n";
		PyErr_Print();
#endif
	} catch (std::exception& e) {
		std::cout << "Core crashed with std::exception.\n";
		std::cout << e.what();
		std::cout << "\n";
	} catch (...) {
		std::cout << "Core crashed with an unknown exception.\n";
	}
}
