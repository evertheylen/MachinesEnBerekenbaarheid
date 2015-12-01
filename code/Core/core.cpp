
// bake_me is in different file

#include <memory>

#include "NGLib/generator/generator.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/python_out/python_outputter.hpp"
#include "NGLib/replacor/replacor.hpp"

int main() {
	std::unique_ptr<PythonOutputter> fo(new PythonOutputter());
	std::unique_ptr<DumbReplacor> dr(new DumbReplacor);
	Generator<DumbReplacor> g(fo.get(), dr.get());
	g.generate({"A", "BC"}, 10);
	//g.loadXML("loadXML.xml");
	//g.saveXML("saveXML.xml");
}
