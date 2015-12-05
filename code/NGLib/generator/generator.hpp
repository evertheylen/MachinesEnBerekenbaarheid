
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
#include <iostream>

#include <exception>
#include <stdexcept>

#include "libs/tinyxml/tinyxml.h"

#include "MBLib/new_CFG/CFG.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/replacor/replacor.hpp"

class exceptionXML: public std::exception {
public:
	virtual std::string syntacticError() const {
		return "The inputXML contains syntactic errors\n";
	}
	virtual std::string noValidFilename() const {
		return "The filename is not valid\n";
	}
	virtual std::string semanticError(std::string fault, std::string correct) const {
		std::string error = "The inputXML contains semantic errors: " + fault + " instead of " + correct + ".\n";
		return error;
	}
} myex;


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
			std::cerr << "Exception caught: " << myex.noValidFilename() << std::endl;
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
	void saveXML(std::string filename) {
		if (!filename.compare("")){
			std::cerr << "Exception caught: " << myex.noValidFilename() << std::endl;
			return;
		}
		TiXmlDocument doc;
		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
		TiXmlElement* root = new TiXmlElement("Generator");
		
		//Outputter
		TiXmlElement* outputter = new TiXmlElement("Outputter");
		outputter->SetAttribute("type", "PythonOutputter");
			//Information about PythonOutputter here
		root->LinkEndChild(outputter);
		
		//CFG replacor
		TiXmlElement* replacor = new TiXmlElement("Replacor");
		replacor->SetAttribute("type", "CfgReplacor");
			//Information about CfgReplacor here
		root->LinkEndChild(replacor);
		
		//Extra setting
		TiXmlElement* extra_setting = new TiXmlElement("extra_setting");
		root->LinkEndChild(extra_setting);
			//Information about extra settings here
		
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
