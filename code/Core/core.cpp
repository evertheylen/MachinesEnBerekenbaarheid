
/* [bake me]

dependencies["build_exec"] = [
	"NGLib/replacor>>build_objects",
	"NGLib/generator>>build_objects",
	"NGLib/outputter>>build_objects",
]

executable = "core"

[stop baking] */

#include "NGLib/generator/generator.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/replacor/replacor.hpp"

int main() {
	Generator g(new FileOutPutter("test"), new DumbReplacor);
	g.generate({"A", "A"}, 10);
}
