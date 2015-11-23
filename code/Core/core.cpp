
/* [bake me]

dependencies["build_exec"] = [
	"NGLib/replacor>>build_objects",
	"NGLib/generator>>build_objects",
	"NGLib/outputter>>build_objects",
]

executable = "core"

[stop baking] */

#include <memory>

#include "NGLib/generator/generator.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/replacor/replacor.hpp"

int main() {
	std::unique_ptr<FileOutPutter> fo(new FileOutPutter("test"));
	std::unique_ptr<DumbReplacor> dr(new DumbReplacor);
	Generator g(fo.get(), dr.get());
	g.generate({"A", "BC"}, 10);
	//g.loadXML("loadXML.xml");
	g.saveXML("saveXML.xml");
}
