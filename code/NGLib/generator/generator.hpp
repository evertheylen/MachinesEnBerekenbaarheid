
#ifndef _GENERATOR_H
#define _GENERATOR_H

/* [bake me]

dependencies["headers"] = [
	"libs/tinyxml>>headers",
	"MBLib/new_CFG>>headers",
	"NGLib/outputter>>headers",
	"NGLib/replacor>>headers",
	"NGLib/exceptions>>headers",
]

[stop baking] */

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <random>

#include "libs/tinyxml/tinyxml.h"

#include "MBLib/new_CFG/CFG.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/python_out/python_outputter.hpp"
#include "NGLib/replacor/stochastic_replacor.hpp"
#include "NGLib/replacor/normal_replacor.hpp"
#include "NGLib/exceptions/exceptions.hpp"



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
		if (!inputfile.compare("")) {
			throw noValidFilename();
		}
		std::random_device rd;
		TiXmlDocument file;
		file.LoadFile(inputfile);
		TiXmlElement* root = file.FirstChildElement();
		if (root == NULL or root->FirstChildElement() == NULL) {
			throw syntacticError();
		}
		std::string rootValue = root->Value();
		if (rootValue.compare("GENERATOR") == 0) {
			for (TiXmlElement* nextchild = root->FirstChildElement(); nextchild != NULL; nextchild = nextchild->NextSiblingElement()) {
				std::string nextchildValue = nextchild->GetText();
				if (nextchildValue.compare("PYTHON_OUTPUTTER") == 0 or nextchildValue.compare("NORMAL_REPLACOR") == 0) {
					if (nextchildValue.compare("PYTHON_OUTPUTTER") == 0) {
						out = FileOutputter("outputFile");
						out->init();
						continue;
					}
					if (nextchildValue.compare("FILE_OUTPUTTER") == 0) {
						out = PythonOutputter("outputPython");
						out->init();
						continue;
					}
					if (nextchildValue.compare("STOCHASTIC_REPLACOR") == 0) {
						repl = StochasticReplacor(nextchild, rd());
						continue;
					}
					if (nextchildValue.compare("NORMAL_REPLACOR") == 0) {
						repl = NormalReplacor(nextchild, rd());
						continue;
					} else {
						throw syntacticError();
					}
				} else {
					throw semanticError(nextchildValue , "OUTPUTTER/NORMAL_REPLACOR");
				}
			}
		} else {
			throw semanticError(rootValue , "GENERATOR");
		}
	}

	// this function will save to an XML file
	void saveXML(std::string filename) {
		if (!filename.compare("")){
			throw noValidFilename();
		}
		TiXmlDocument doc;
		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
		TiXmlElement* root = new TiXmlElement("GENERATOR");
		
		//CFG replacor
		TiXmlElement* replacor = repl->to_xml();
		root->LinkEndChild(replacor);
		
		//Outputter
		TiXmlElement* outputter = out->to_xml();
		root->LinkEndChild(outputter);
		
		doc.LinkEndChild(decl);
		doc.LinkEndChild(root);
		
		doc.SaveFile(filename);
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
