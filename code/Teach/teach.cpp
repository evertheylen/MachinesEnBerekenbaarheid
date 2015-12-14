
/* [bake me]

dependencies["build_exec"] = [
    "NGLib/replacor>>build_objects",
    "NGLib/generator>>build_objects",
    "NGLib/outputter>>build_objects",
    "NGLib/python_out>>build_objects",
    "MBLib/common>>build_objects",
    "NGLib/teacher>>headers",
    "NGLib/teacher>>build_objects",
]

gcc_config = {
    "pre_extra": "-Wno-deprecated -I%s -Ilibs/boost/b2stuff/include/"%project_config["python_include_dir"],
    "post_extra": "-Wl,-Bstatic -Llibs/boost/b2stuff/lib/ -lboost_python -Wl,-Bdynamic -L{python_lib_dir} -l{python_lib_name} -lutil -ldl -lm".format(**project_config)
}


executable = "teach"

[stop baking] */

#include <memory>
#include <exception>
#include <random>

#include "boost/python.hpp"

#include "NGLib/generator/generator.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/python_out/python_outputter.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "NGLib/replacor/context_replacor.hpp"
#include "MBLib/common/common.hpp"
#include "NGLib/teacher/teacher.hpp"
#include "libs/tinyxml/tinyxml.h"
#include <string>

int main(int argc, char** argv) {
	// load xml, construct a Teacher
	
	// while not exited
		// create a tree (tree = Teacher.generate(...))
		// display it to the user (Teacher.output(tree, out))
		// input score, use Teacher.score method
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
	try {
		std::unique_ptr<GeneratorInterface> g(loadXML(args.at(0)));
		std::vector<std::string> startstring = split(args.at(1));
		int max_depth = args.size() == 3? std::stoi(args.at(2)) : -1;
		Generator<ContextReplacor>* generator = dynamic_cast<Generator<ContextReplacor>*>(g.get());
		
		Teacher t(generator->get_replacor());
		while (true) {
			auto tree = t.generate(args[1]);
			//std::unique_ptr<PythonOutputter> out(new PythonOutputter("output.py"));
			//out->init();
			//t.output(tree, out.get());
			std::cout << tree << std::endl;
			std::string score;
			std::cout << "Please input a score for this tree:  (PRESS q TO EXIT)\n";
			std::getline(std::cin, score);
			if (score == "q") {
				break;
			} else {
				t.score(tree, std::stod(score));
				generator->saveXML(args[0]);
			}
		}
		std::cout << "Exiting and saving to " << args[0] << " ...\n";		
	} catch (boost::python::error_already_set e) {
		std::cout << "Core crashed with Python Exception:\n";
		PyErr_Print();
	}
	
 	catch (std::exception& e) {
 		std::cout << "Core crashed with std::exception.\n";
 		std::cout << e.what();
 		std::cout << "\n";
 	} catch (...) {
 		std::cout << "Core crashed with an unknown exception.\n";
 	}
}
