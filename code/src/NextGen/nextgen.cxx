
#include <memory>
#include <exception>
#include <random>
#include <string>
#include <vector>

#include "boost/python.hpp"
#include "libs/tinyxml/tinyxml.h"
#include "libs/linenoise/linenoise.h"

#include "MBLib/common/common.hpp"
#include "MBLib/Earley/earley.hpp"
#include "NGLib/generator/generator.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/python_out/python_outputter.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "NGLib/replacor/context_replacor.hpp"
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

void completion(const char *buf, linenoiseCompletions *lc) {
	switch (buf[0]) {
		case 'b':
			linenoiseAddCompletion(lc,"build");
			break;
		case 't':
			linenoiseAddCompletion(lc,"tree");
			break;
		case 'p':
			linenoiseAddCompletion(lc,"print");
			break;
		case 'o':
			linenoiseAddCompletion(lc,"output");
			break;
		case 's':
			linenoiseAddCompletion(lc,"score");
			linenoiseAddCompletion(lc,"save");
			break;
		case 'c':
			linenoiseAddCompletion(lc,"clear");
			break;
		case 'q':
			linenoiseAddCompletion(lc, "quit");
			break;
		case 'h':
			linenoiseAddCompletion(lc, "help");
			break;
		case 'e':
			linenoiseAddCompletion(lc, "earley");
			break;
		default:
			break;
	}
}

int main(int argc, char** argv) {
	// load xml, construct a Teacher
	
	std::cout
	<< " _        _______          _________ _______  _______  _       \n"
	<< "( (    /|(  ____ \\|\\     /|\\__   __/(  ____ \\(  ____ \\( (    /|\n"
	<< "|  \\  ( || (    \\/( \\   / )   ) (   | (    \\/| (    \\/|  \\  ( |\n"
	<< "|   \\ | || (__     \\ (_) /    | |   | |      | (__    |   \\ | |\n"
	<< "| (\\ \\) ||  __)     ) _ (     | |   | | ____ |  __)   | (\\ \\) |\n"
	<< "| | \\   || (       / ( ) \\    | |   | | \\_  )| (      | | \\   |\n"
	<< "| )  \\  || (____/\\( /   \\ )   | |   | (___) || (____/\\| )  \\  |\n"
	<< "|/    )_)(_______/|/     \\|   )_(   (_______)(_______/|/    )_)\n\n";
                                                               
	
	// while not exited
		// create a tree (tree = Teacher.generate(...))
		// display it to the user (Teacher.output(tree, out))
		// input score, use Teacher.score method
	std::vector<std::string> args;
	for (int i=1; i<argc; i++) {
		args.push_back(argv[i]);
	}
	
	if (args.size() < 1) {
		std::cout << "NextGen <filename>.xml\n\n";
		std::cout << "Please provide a filename.\n";
		return 1;
	}
	
	try {
		Generator g(args[0]);
		
		Teacher t(g.get_replacor());
		std::unique_ptr<Teacher::Teacher3> tree;
		std::vector<std::string> cmds;
		std::string input;
		
		linenoiseSetCompletionCallback(completion);
		char* line;
		
		while (true) {
			if (tree.get()) line = linenoise("[T] ");
			else line = linenoise("[ ] ");
			
			if (line == nullptr) continue;
			input = std::string(line);
			free(line);
			
			cmds = split_string(input, ' ');
			
			if (cmds.size() == 0) continue;
			linenoiseHistoryAdd(input.c_str());
			if (cmds[0] == "build") {
				if (cmds.size() < 2) {
					std::cout << "'build' requires one start symbol\n";
					continue;
				}
				std::string startsymbol = cmds[1];
				
				int max_repl = -1;
				if (cmds.size() == 3) {
					try {
						max_repl = std::stod(cmds[2]);
					} catch (std::invalid_argument& e) {
						std::cout << "Couldn't parse number\n";
						continue;
					}
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
				g.get_outputter()->init();
				tree->leaves([&](Teacher::Element3& p) { g.get_outputter()->output(p.first); });
				g.get_outputter()->close();
			} else if (cmds[0] == "score") {
				if (tree.get() == nullptr) continue;
				if (cmds.size() == 1) {
					std::cout << "'score' needs a score.\n";
					continue;
				}
				double score;
				try {
					score = std::stod(cmds[1]);
				} catch (std::invalid_argument& e) {
					std::cout << "Couldn't parse number\n";
					continue;
				}
				t.score(*tree, score);
			} else if (cmds[0] == "earley") {
				if (cmds.size() == 1) {
					std::cout << "'earley' needs a string.\n";
					continue;
				}
				// TODO get CFG out of replacor someway (interface?)
				// auto s = earley(..., input_string?)
				// TODO select right tree (smallest?)
				// set tree (if there is one)
			} else if (cmds[0] == "clear") {
				tree.reset(nullptr);
			} else if (cmds[0] == "save") {
				g.saveXML(args[0]);
			} else if (cmds[0] == "help") {
				std::cout << "Possible commands:\n\n"
				<< " - Generate new tree:\n"
					<< "\tbuild startsymbol [recursion depth]\n"
				<< " - Print generated tree:\n"
					<< "\ttree\n"
				<< " - Print generated output:\n"
					<< "\tprint\n"
				<< " - Export generated output to specified format:\n"
					<< "\toutput\n"
				<< " - Give a score to the tree (Works only with context replacor):\n"
					<< "\tscore [double score]\n"
				<< " - Clear the generated tree:\n"
					<< "\tclear\n"
				<< " - Save the updated xml (Saves automatically on shutdown):\n"
					<< "\tsave\n"
				<< " - Display this help function:\n"
					<< "\thelp\n"
				<< " - Quit the program:\n"
					<< "\tquit\n";
			} else if (cmds[0] == "quit") {
				break;
			} else {
				std::cout << "I did not understand that.\n";
				std::cout << "Type 'help' for help. Obviously.\n";
			}
		}
		
		std::cout << "Exiting and saving to " << args[0] << " ...\n";
		g.saveXML(args[0]);
	
	} catch (boost::python::error_already_set e) {
		std::cout << "NextGen crashed with Python Exception:\n";
		PyErr_Print();
	} catch (std::exception& e) {
 		std::cout << "NextGen crashed with std::exception.\n";
 		std::cout << e.what();
 		std::cout << "\n";
 	} catch (...) {
 		std::cout << "NextGen crashed with an unknown exception.\n";
 	}
}
