
#ifndef _GENERATOR_H
#define _GENERATOR_H

/* [bake me]

# build_objects dependencies because of forward declarations

dependencies["headers"] = [
	"libs/tinyxml>>headers"
]

dependencies["build_objects"] = [
	"NGLib/outputter>>build_objects",
	"NGLib/replacor>>build_objects"
]

[stop baking] */

#include <list>
#include <vector>
#include <string>

// forward declarations
class Outputter;
class Replacor;

class Generator {
public:
	Generator(Outputter* _out, Replacor* _repl);

	// if max_repl == -1 ==> Infinite
	void generate(std::vector<std::string> start, int max_repl=-1);

	// this function will load a specified XML file
	void loadXML(std::string loadfile);

	// this function will save to an XML file
	void saveXML(std::string savefile);

private:
	void rec_generate(std::string s, std::list<std::string>& context, int max_repl);

	Outputter* out;
	Replacor* repl;
};


#endif
