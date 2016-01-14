
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
#include "NGLib/replacor/context_replacor.hpp"
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
		std::list<typename ReplacorT::Rule_Type::NumT> context;
		out->init();
		for (std::string& s: start) {
			rec_generate(s, context, max_repl);
		}
		out->close();
	}

	// this function will save to an XML file
	void saveXML(std::string filename) {
		if (filename == "") {
			throw NoValidFilename(filename);
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
	
	ReplacorT* get_replacor() const {
		return repl.get();
	}
	
private:
	void rec_generate(std::string s, std::list<typename ReplacorT::Rule_Type::NumT>& context, int max_repl) {
		//std::cout << "generating for var " << s << ", with max_repl " << max_repl << " and context.size() " << context.size() << "\n";
		for (unsigned int i=0; i<context.size(); i++) std::cout << "  ";
		if ((max_repl != -1) and (int(context.size()) == max_repl)) {
			// we have reached the maximum depth, output this symbol (variable or not)
			//std::cout << "   --> reached max depth because context.size() >= max_repl: " << context.size() << " >= " << max_repl << "\n";
			out->output(s);
			std::cout << "[" << s << "]\n";
		} else if (repl->replaceable(s)) {
			//std::cout << "   --> replacing!\n";
			// replace and continue with recursive stuff
			// also add to context
			std::cout << s << ": \n";
			auto num = repl->replace(s, context);
			context.push_back(num);
			for (auto sub_s: repl->get_body(num)) {
				rec_generate(sub_s, context, max_repl);
			}
			context.pop_back(); // reference!
		} else {
			// not replaceable, output
			//std::cout << "   --> not replacable\n";
			out->output(s);
			std::cout << "[" << s << "]\n";
		}
	}
	
	std::unique_ptr<Outputter> out;
	
	// templated
	std::unique_ptr<ReplacorT> repl;
};

GeneratorInterface* loadXML(std::string inputfile);
