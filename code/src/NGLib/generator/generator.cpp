
#include "generator.hpp"

Generator::Generator() {}

Generator::Generator(Replacor* _repl, Outputter* _out):
	repl(_repl), out(_out) {}

Generator::Generator(const std::string& inputfile) {
	if (inputfile == "") {
		throw NoValidFilename(inputfile);
	}
	std::random_device rd;
	TiXmlDocument file;
	if (not file.LoadFile(inputfile)) throw NGException("Something went wrong while loading XML file");
	TiXmlElement* root = file.FirstChildElement();
	if (root == nullptr or root->FirstChildElement() == nullptr) {
		throw SyntacticError("no root or root has no content");
	}
	std::string rootValue = root->Value();
	if (rootValue.compare("GENERATOR") == 0) {
		for (TiXmlElement* nextchild = root->FirstChildElement(); nextchild != nullptr; nextchild = nextchild->NextSiblingElement()) {
			std::string nextchildValue = nextchild->Value();
			if (nextchildValue == "PYTHON_OUTPUTTER") {
				out.reset(new PythonOutputter(nextchild));
			} else if (nextchildValue == "FILE_OUTPUTTER") {
				out.reset(new FileOutputter(nextchild));
			} else if (nextchildValue == "STOCHASTIC_REPLACOR") {
				// create generator with StochasticReplacor as ReplacorT
				repl.reset(new StochasticReplacor(nextchild, rd()));;
			} else if (nextchildValue == "CONTEXT_REPLACOR") {
				repl.reset(new ContextReplacor(nextchild, rd()));
			} else if (nextchildValue == "NORMAL_REPLACOR") {
				repl.reset(new NormalReplacor(nextchild, rd()));
			} else {
				throw SyntacticError(std::string("Wrong kind of replacor: ") + nextchildValue);
			}
		}
		if (out.get() == nullptr) throw NGException("Could not find outputter");
		if (repl.get() == nullptr) throw NGException("Could not find replacor");
	} else {
		throw SemanticError(rootValue, "GENERATOR");
	}
}


// if max_repl == -1 ==> Infinite
void Generator::generate(std::vector<std::string> start, int max_repl) {
	std::list<unsigned int> context;
	out->init();
	for (std::string& s: start) {
		rec_generate(s, context, max_repl);
	}
	out->close();
}

// this function will save to an XML file
void Generator::saveXML(std::string filename) {
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

void Generator::setOutputter(Outputter* _out) {
	out = std::unique_ptr<Outputter>(_out);
}

Replacor* Generator::get_replacor() const {
	return repl.get();
}

Outputter* Generator::get_outputter() const {
	return out.get();
}

void Generator::rec_generate(std::string s, std::list<unsigned int>& context, int max_repl) {
	//std::cout << "generating for var " << s << ", with max_repl " << max_repl << " and context.size() " << context.size() << "\n";
// 	for (unsigned int i=0; i<context.size(); i++) std::cout << "  ";
	if ((max_repl != -1) and (int(context.size()) == max_repl)) {
		// we have reached the maximum depth, output this symbol (variable or not)
		//std::cout << "   --> reached max depth because context.size() >= max_repl: " << context.size() << " >= " << max_repl << "\n";
		out->output(s);
// 		std::cout << "[" << s << "]\n";
	} else if (repl->replaceable(s)) {
		//std::cout << "   --> replacing!\n";
		// replace and continue with recursive stuff
		// also add to context
// 		std::cout << s << ": \n";
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
// 		std::cout << "[" << s << "]\n";
	}
}

