
#pragma once

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
#include <memory>

#include "libs/tinyxml/tinyxml.h"

#include "MBLib/new_CFG/CFG.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/python_out/python_outputter.hpp"
#include "NGLib/replacor/stochastic_replacor.hpp"
#include "NGLib/replacor/normal_replacor.hpp"
#include "NGLib/exceptions/exceptions.hpp"


class GeneratorInterface {
public:
	virtual void generate(std::vector<std::string> start, int max_repl = -1) = 0;
	virtual void saveXML(std::string filename) = 0;
	
	virtual void setOutputter(Outputter* out) = 0;
	
	virtual ~GeneratorInterface() {};
};


template <typename ReplacorT>
class Generator: public GeneratorInterface {
public:

	Generator() {}
	
	Generator(ReplacorT* _repl):
		repl(_repl) {}


	// if max_repl == -1 ==> Infinite
	void generate(std::vector<std::string> start, int max_repl = -1) {
		std::list<typename ReplacorT::Rule_Type> context;
		out->init();
		for (auto s : start) {
			rec_generate(s, context, max_repl);
		}
		out->close();
	}

	// this function will save to an XML file
	void saveXML(std::string filename) {
		if (!filename.compare("")) {
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
	
	void setOutputter(Outputter* _out) {
		out = std::unique_ptr<Outputter>(_out);
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

	std::unique_ptr<Outputter> out;

	// templated
	std::unique_ptr<ReplacorT> repl;
};

GeneratorInterface* loadXML(std::string inputfile);
