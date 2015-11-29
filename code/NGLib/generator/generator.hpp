
#ifndef _GENERATOR_H
#define _GENERATOR_H

/* [bake me]

dependencies["headers"] = [
	"libs/tinyxml>>headers",
	"MBLib/new_CFG>>headers",
	"NGLib/outputter>>headers",
	"NGLib/replacor>>headers"
]

[stop baking] */

#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <exception>

#include "libs/tinyxml/tinyxml.h"

#include "MBLib/new_CFG/CFG.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/replacor/replacor.hpp"

class exceptionXML: public std::exception {
public:
	virtual std::string syntacticError() const throw() {
		return "The inputXML contains syntactic errors\n";
	}
	virtual std::string semanticError(std::string fault, std::string correct) const throw() {
		std::string error = "The inputXML contains syntactic errors: " + fault + " instead of " + correct + ".\n";
		return error;
	}
};


template <typename ReplacorT>
class Generator {
public:

	Generator(Outputter* _out, ReplacorT* _repl):
		out(_out), repl(_repl) {}


	// if max_repl == -1 ==> Infinite
	void generate(std::vector<std::string> start, int max_repl = -1) {
		std::list<typename ReplacorT::Rule_Type> context;
		out->init();
		for (auto s : start) {
			rec_generate(s, context, max_repl);
		}
		out->close();
	}

	// this function will load a specified XML file
	void loadXML(std::string inputfile) {
		exceptionXML myex;
		if (inputfile == "") {
			std::cout << "There is no file specified.\n";
			return;
		}
		TiXmlDocument file;
		file.LoadFile(inputfile);
		TiXmlElement* root = file.FirstChildElement();
		if (root == NULL or root->FirstChildElement() == NULL) {
			std::cerr << "Exception caught: " << myex.syntacticError() << std::endl;
			return;
		}
		std::string rootValue = root->Value();
		if (rootValue.compare("Generator") == 0) {
			for (TiXmlElement* nextchild = root->FirstChildElement(); nextchild != NULL; nextchild = nextchild->NextSiblingElement()) {
				std::string nextchildValue = nextchild->Value();
				if (nextchildValue.compare("Outputter") == 0 or nextchildValue.compare("Replacor") == 0 or nextchildValue.compare("extra_setting") == 0) {
					if (nextchildValue.compare("extra_setting") == 0) {
						//Do something with value here
					} else {
						if (nextchildValue.compare("Outputter") == 0  and nextchild->Attribute("type") != NULL) {
							std::string nextchildAttribute = nextchild->Attribute("type");
							if (nextchildAttribute.compare("PythonOutputter") == 0) {
								//Process input through PythonOutputter
								continue;
							} else {
								std::cerr << "Exception caught: " << myex.semanticError(nextchildAttribute , "PythonOutputter") << std::endl;
								return;
							}
						}
						if (nextchildValue.compare("Replacor") == 0  and nextchild->Attribute("type") != NULL) {
							std::string nextchildAttribute = nextchild->Attribute("type");
							if (nextchildAttribute.compare("CfgReplacor") == 0) {
								//Process input through CfgReplacor
								continue;
							} else {
								std::cerr << "Exception caught: " << myex.semanticError(nextchildAttribute , "CfgReplacor") << std::endl;
								return;
							}
						} else {
							std::cerr << "Exception caught: " << myex.syntacticError() << std::endl;
							return;
						}
					}
				} else {
					std::cerr << "Exception caught: " << myex.semanticError(nextchildValue , "Outputter/Replacor/extra_setting") << std::endl;
					return;
				}
			}
		} else {
			std::cerr << "Exception caught: " << myex.semanticError(rootValue , "Generator") << std::endl;
			return;
		}
	}

	// this function will save to an XML file
	void saveXML(std::string savefile) {
		std::ofstream output(savefile);
		if (output.is_open()) {
			output << "<?xml version=" << '"' << "1.0" << '"' << "?>\n";
			output << "<Generator>\n";

			output << "	<Outputter type=" << '"' << "PythonOutputter" << '"' << ">\n";
			output << "		Information about PythonOutputter here\n";
			output << "	</Outputter>\n";

			output << "	<Replacor type=" << '"' << "CfgReplacor" << '"' << ">\n";
			output << "		Information about CfgReplacor here\n";
			output << "	</Replacor>\n";

			output << "	<extra_setting>\n";
			output << "		Information about extra settings here\n";
			output << "	</extra_setting>\n";

			output << "</Generator>";
			output.close();
		} else {
			std::cout << "There was no valid XML save file created\n";
		}
	}

private:
	void rec_generate(std::string s, std::list<typename ReplacorT::Rule_Type>& context, int max_repl) {
		if (max_repl > 0 && context.size() >= max_repl) {
			// we have reached the maximum depth, output this symbol (variable or not)
			out->output(s);
		} else if (repl->replaceable(s)) {
			// replace and continue with recursive stuff
			typename ReplacorT::Rule_Type r = repl->replace(s, context);
			context.push_back(r);
			for (auto sub_s : r.get_body()) {
				rec_generate(sub_s, context, max_repl - 1);
			}
			context.pop_back(); // reference!
		} else {
			// not replaceable, output
			out->output(s);
		}
	}

	Outputter* out;

	// templated
	ReplacorT* repl;
};


#endif
