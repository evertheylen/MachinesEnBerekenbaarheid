
#include "generator.hpp"

#include "NGLib/outputter/outputter.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "libs/tinyxml/tinyxml.h"
#include <fstream>
#include <iostream>


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
	if (inputfile == ""){
		std::cout << "There is no file specified.\n";
		return;
	}
	TiXmlDocument file;
	file.LoadFile(inputfile);
	//inputXML.LoadFile(loadfile);
	// std::ifstream myfile (loadfile);
	// std::string line;
	// if (myfile.is_open()){
	// 	while ( getline (myfile,line) ){
	// 		std::cout << line << '\n';
	// 	}
	// 	myfile.close();
	// }else{
	// 	std::cout << "The specified XML file isn't valid\n";
	// }
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

		output << "	<extra_setting>";
		//Insert a randow value for extra setting here TODO
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
