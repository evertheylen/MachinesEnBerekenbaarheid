
#include "generator.hpp"

#include "NGLib/outputter/outputter.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "libs/tinyxml/tinyxml.h"
#include <fstream>
#include <string>
#include <iostream>
#include <exception>

class exceptionXML: public std::exception{
	public:
	  virtual std::string syntacticError() const throw(){
	    return "The inputXML contains syntactic errors\n";
	  }
		virtual std::string semanticError(std::string fault, std::string correct) const throw(){
		 	std::string error = "The inputXML contains syntactic errors: " + fault + " instead of " + correct + ".\n";
			return error;
	  }
};

Generator::Generator(Outputter* _out, Replacor* _repl):
	out(_out), repl(_repl) {}

// if max_repl == -1 ==> Infinite (default)
void Generator::generate(std::vector<std::string> start, int max_repl) {
	std::list<std::string> context = {};
	out->init();
	for (auto s: start) {
		rec_generate(s, context, max_repl);
	}
	out->close();
}

// this function will load a specified XML file
void Generator::loadXML(std::string inputfile){
	exceptionXML myex;
	if (inputfile == ""){
		std::cout << "There is no file specified.\n";
		return;
	}
	TiXmlDocument file;
	file.LoadFile(inputfile);
	TiXmlElement* root = file.FirstChildElement();
	if (root == NULL or root->FirstChildElement() == NULL){
			std::cerr << "Exception caught: " << myex.syntacticError() << std::endl;
			return;
	}
	std::string rootValue = root->Value();
	if(rootValue.compare("Generator") == 0){
		for( TiXmlElement* nextchild = root->FirstChildElement(); nextchild!= NULL; nextchild = nextchild->NextSiblingElement()){
			std::string nextchildValue = nextchild->Value();
			if(nextchildValue.compare("Outputter") == 0 or nextchildValue.compare("Replacor") == 0 or nextchildValue.compare("extra_setting") == 0){
				if(nextchildValue.compare("extra_setting") == 0){
					//Do something with value here
				}else{
					if(nextchildValue.compare("Outputter") == 0  and nextchild->Attribute("type") != NULL){
						std::string nextchildAttribute = nextchild->Attribute("type");
						if(nextchildAttribute.compare("PythonOutputter") == 0){
							//Process input through PythonOutputter
							continue;
						}else{
							std::cerr << "Exception caught: " << myex.semanticError(nextchildAttribute , "PythonOutputter") << std::endl;
							return;
						}
					}
					if(nextchildValue.compare("Replacor") == 0  and nextchild->Attribute("type") != NULL){
						std::string nextchildAttribute = nextchild->Attribute("type");
						if(nextchildAttribute.compare("CfgReplacor") == 0){
							//Process input through CfgReplacor
							continue;
						}else{
							std::cerr << "Exception caught: " << myex.semanticError(nextchildAttribute , "CfgReplacor") << std::endl;
							return;
						}
					}else{
						std::cerr << "Exception caught: " << myex.syntacticError() << std::endl;
						return;
					}
				}
			}else{
				std::cerr << "Exception caught: " << myex.semanticError(nextchildValue , "Outputter/Replacor/extra_setting") << std::endl;
				return;
			}
		}
	}else{
		std::cerr << "Exception caught: " << myex.semanticError(rootValue , "Generator") << std::endl;
		return;
	}
}

// this function will save to an XML file
void Generator::saveXML(std::string savefile){
	std::ofstream output (savefile);
	if (output.is_open()){
    output << "<?xml version=" << '"' << "1.0" << '"' << "?>\n";
		output << "<Generator>\n";

		output << "	<Outputter type=" << '"' << "PythonOutputter" << '"' << ">\n";
		output<< "		Information about PythonOutputter here\n";
		output << "	</Outputter>\n";

		output << "	<Replacor type=" << '"' << "CfgReplacor" << '"' << ">\n";
		output<< "		Information about CfgReplacor here\n";
		output << "	</Replacor>\n";

		output << "	<extra_setting>\n";
		output<< "		Information about extra settings here\n";
		output << "	</extra_setting>\n";

		output<<"</Generator>";
    output.close();
  }else{
		std::cout << "There was no valid XML save file created\n";
	}
}

void Generator::rec_generate(std::string s, std::list<std::string>& context, int max_repl) {
	if (max_repl != -1 && context.size() >= max_repl) {
		// we have reached the maximum depth, output this symbol (variable or not)
		out->output(s);
	} else if (repl->replaceable(s)) {
		// replace and continue with recursive stuff
		context.push_back(s);
		std::vector<std::string> body = repl->replace(s, context);
		for (auto sub_s: body) {
			rec_generate(sub_s, context, max_repl);
		}
		context.pop_back(); // reference!
	} else {
		// not replaceable, output
		out->output(s);
	}
}
