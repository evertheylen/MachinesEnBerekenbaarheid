
#include <memory>
#include <exception>
#include <random>
#include <string>
#include <vector>

#include "boost/python.hpp"
#include "libs/tinyxml/tinyxml.h"

#include "NGLib/generator/generator.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/python_out/python_outputter.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "NGLib/replacor/context_replacor.hpp"
#include "MBLib/common/common.hpp"
#include "NGLib/teacher/teacher.hpp"
#include "NGLib/exceptions/exceptions.hpp"

std::vector<std::string> split_string(const std::string& s, char token) {
	std::string buf;
	std::vector<std::string> result;
	for (char c: s) {
		if (c == token) {
			result.push_back(buf);
			buf.clear();
		} else {
			buf += c;
		}
	}
	if (buf.length() > 0) {
		result.push_back(buf);
	}
	return result;
}

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
	
	if (args.size() < 1) {
		std::cout << "teach <filename>.xml\n\n";
		std::cout << "Please provide a filename.\n";
		return 1;
	}
	
	TiXmlDocument doc;
	try {
		if (not doc.LoadFile(args[0])) throw NoValidFilename(args[0]);
		std::unique_ptr<GeneratorInterface> g(loadXML(args.at(0)));
		Generator<ContextReplacor>* generator = dynamic_cast<Generator<ContextReplacor>*>(g.get());
		if (generator == nullptr) {
			// TODO work with any generator
			throw NGException("Did not get a Context Replacor xml.");
		}
		
		Teacher t(generator->get_replacor());
		std::unique_ptr<Teacher::Teacher3> tree;
		std::vector<std::string> cmds;
		std::string input;
		while (true) {
// 			auto tree = t.generate(args[1], max_depth);
// 			//std::unique_ptr<PythonOutputter> out(new PythonOutputter("output.py"));
// 			//out->init();
// 			//t.output(tree, out.get());
// 			std::cout << tree << std::endl;
// 			std::string score;
// 			std::cout << "Please input a score for this tree:  (PRESS q TO EXIT)\n";
// 			std::getline(std::cin, score);
// 			if (score == "quit") {
// 				break;
// 			} else {
// 				t.score(tree, std::stod(score));
// 				generator->saveXML(args[0]);
// 			}
			
			if (tree.get()) std::cout << "[T] ";
			else std::cout << "[ ] ";
			std::getline(std::cin, input);
			cmds = split_string(input, ' ');
			
			if (cmds.size() == 0) continue;
			if (cmds[0] == "build") {
				if (cmds.size() < 2) {
					std::cout << "'build' requires one start symbol\n";
					continue;
				}
				std::string startsymbol = cmds[1];
				
				int max_repl = -1;
				if (cmds.size() == 3) {
					max_repl = std::stod(cmds[2]);
					// TODO proper errors?
				}
				
				tree.reset(t.generate(startsymbol, max_repl));
			} else if (cmds[0] == "tree") {
				if (tree.get() == nullptr) continue;
				std::cout << *tree.get();
			} else if (cmds[0] == "print") {
				if (tree.get() == nullptr) continue;
				tree->leaves([&](Teacher::Element3& p) { std::cout << p.first; });
				std::cout << "\n";
			} else if (cmds[0] == "output") {
				if (tree.get() == nullptr) continue;
				generator->get_outputter()->init();
				tree->leaves([&](Teacher::Element3& p) { generator->get_outputter()->output(p.first); });
				generator->get_outputter()->close();
			} else if (cmds[0] == "score") {
				if (tree.get() == nullptr) continue;
				if (cmds.size() == 1) {
					std::cout << "'score' needs a score.\n";
					continue;
				}
				double score = std::stod(cmds[1]);
				t.score(*tree, score);
			} else if (cmds[0] == "clear") {
				tree.reset(nullptr);
			} else if (cmds[0] == "save") {
				generator->saveXML(args[0]);
			} else if (cmds[0] == "help") {
				std::cout << "help here\n"; // TODO
			} else if (cmds[0] == "quit") {
				break;
			} else {
				std::cout << "I did not understand that.\n";
				std::cout << "Type 'help' for help. Obviously.\n";
			}
		}
				
		std::cout << "Exiting and saving to " << args[0] << " ...\n";
		generator->saveXML(args[0]);
	} catch (boost::python::error_already_set e) {
		std::cout << "Core crashed with Python Exception:\n";
		PyErr_Print();
	} catch (std::exception& e) {
 		std::cout << "Core crashed with std::exception.\n";
 		std::cout << e.what();
 		std::cout << "\n";
 	} catch (...) {
 		std::cout << "Core crashed with an unknown exception.\n";
 	}
}
