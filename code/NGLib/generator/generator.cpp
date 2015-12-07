
#include "generator.hpp"

GeneratorInterface* loadXML(std::string inputfile) {
	std::cout << "starting parsing\n";
	GeneratorInterface* gen = nullptr;
	Outputter* out = nullptr;
	
	if (!inputfile.compare("")) {
		throw noValidFilename();
	}
	std::random_device rd;
	TiXmlDocument file;
	file.LoadFile(inputfile);
	TiXmlElement* root = file.FirstChildElement();
	if (root == nullptr or root->FirstChildElement() == nullptr) {
		throw syntacticError();
	}
	std::string rootValue = root->Value();
	if (rootValue.compare("GENERATOR") == 0) {
		for (TiXmlElement* nextchild = root->FirstChildElement(); nextchild != nullptr; nextchild = nextchild->NextSiblingElement()) {
			std::string nextchildValue = nextchild->Value();
			if (nextchildValue.compare("PYTHON_OUTPUTTER") == 0 or nextchildValue.compare("FILE_OUTPUTTER") == 0 or nextchildValue.compare("STOCHASTIC_REPLACOR") == 0 or nextchildValue.compare("NORMAL_REPLACOR") == 0) {
				if (nextchildValue.compare("PYTHON_OUTPUTTER") == 0) {
					delete out;
					out = new PythonOutputter(nextchild);
					continue;
				}
				if (nextchildValue.compare("FILE_OUTPUTTER") == 0) {
					delete out;
					out = new FileOutputter(nextchild);
					continue;
				}
				if (nextchildValue.compare("STOCHASTIC_REPLACOR") == 0) {
					delete gen;
					// create generator with StochasticReplacor as ReplacorT
					gen = new Generator<StochasticReplacor>(new StochasticReplacor(nextchild, rd()));
					continue;
				}
				if (nextchildValue.compare("NORMAL_REPLACOR") == 0) {
					delete gen;
					// create generator with NormalReplacor as ReplacorT
					gen = new Generator<NormalReplacor>(new NormalReplacor(nextchild, rd()));
					continue;
				} else {
					throw syntacticError();
				}
			} else {
				throw semanticError(nextchildValue , "PYTHON_OUTPUTTER/FILE_OUTPUTTER/STOCHASTIC_REPLACOR/NORMAL_REPLACOR");
			}
		}
		assert(out != nullptr);
		gen->setOutputter(out);
		return gen;
	} else {
		throw semanticError(rootValue , "GENERATOR");
	}
}


