
// bake_me is in different file

#include <memory>
#include <exception>
#include <random>

#include "boost/python.hpp"

#include "NGLib/generator/generator.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/python_out/python_outputter.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "MBLib/common/common.hpp"
#include "NGLib/teacher/teacher.hpp"

int main(int argc, char** argv) {
	std::vector<std::string> args;
	for (int i=1; i<argc; i++) {
		args.push_back(argv[i]);
	}
	
	if (args.size() < 2) {
		std::cout << "core <filename>.xml <startstring> [max_depth]\n\n";
		std::cout << "Please provide a filename and at least one start symbol.\n";
		std::cout << "You can also provide a maximum recursion depth.\n";
		return 1;
	}
	
	TiXmlDocument doc;
	assert(doc.LoadFile(args[0]));
	std::random_device rd;
	ContextReplacor ctxrepl(doc.RootElement()->FirstChildElement("CONTEXT_REPLACOR"), rd());
	Teacher t(ctxrepl);
	auto tree = t.generate(args[1]);
	std::unique_ptr<PythonOutputter> out(new PythonOutputter("output.py"));
	out->init();
	t.output(tree, out.get());
	
	/*
	
	try {
		std::unique_ptr<GeneratorInterface> g(loadXML(args.at(0)));
		std::vector<std::string> startstring = split(args.at(1));
		int max_depth = args.size() == 3? std::stoi(args.at(2)) : -1;
		g->generate(startstring, max_depth);
		
		//g->saveXML("saveXML.xml");
	} catch (boost::python::error_already_set e) {
		std::cout << "Core crashed with Python Exception:\n";
		PyErr_Print();
	} 
	
	*/
	
// 	catch (std::exception& e) {
// 		std::cout << "Core crashed with std::exception.\n";
// 		std::cout << e.what();
// 		std::cout << "\n";
// 	} catch (...) {
// 		std::cout << "Core crashed with an unknown exception.\n";
// 	}
}
