
#include "generator.hpp"

GeneratorInterface* loadXML(std::string inputfile) {
	GeneratorInterface* gen = nullptr;
	Outputter* out = nullptr;
	
	if (inputfile == "") {
		throw NoValidFilename(inputfile);
	}
	std::random_device rd;
	TiXmlDocument file;
	assert(file.LoadFile(inputfile));
	TiXmlElement* root = file.FirstChildElement();
	if (root == nullptr or root->FirstChildElement() == nullptr) {
		throw SyntacticError("no root or root has no content");
	}
	std::string rootValue = root->Value();
	if (rootValue.compare("GENERATOR") == 0) {
		for (TiXmlElement* nextchild = root->FirstChildElement(); nextchild != nullptr; nextchild = nextchild->NextSiblingElement()) {
			std::string nextchildValue = nextchild->Value();
			if (nextchildValue == "PYTHON_OUTPUTTER") {
				delete out;
				out = new PythonOutputter(nextchild);
				continue;
			} else if (nextchildValue == "FILE_OUTPUTTER") {
				delete out;
				out = new FileOutputter(nextchild);
				continue;
			} else if (nextchildValue == "STOCHASTIC_REPLACOR") {
				delete gen;
				// create generator with StochasticReplacor as ReplacorT
				gen = new Generator<StochasticReplacor>(new StochasticReplacor(nextchild, rd()));
				continue;
			} else if (nextchildValue == "CONTEXT_REPLACOR") {
				delete gen;
				gen = new Generator<ContextReplacor>(new ContextReplacor(nextchild, rd()));
			} else if (nextchildValue == "NORMAL_REPLACOR") {
				delete gen;
				// create generator with NormalReplacor as ReplacorT
				gen = new Generator<NormalReplacor>(new NormalReplacor(nextchild, rd()));
				continue;
			} else {
				throw SyntacticError(std::string("Wrong kind of replacor: ") + nextchildValue);
			}
		}
		assert(out != nullptr);
		gen->setOutputter(out);
		gen->saveXML("intermediate.xml");
		return gen;
	} else {
		throw SemanticError(rootValue, "GENERATOR");
	}
}


