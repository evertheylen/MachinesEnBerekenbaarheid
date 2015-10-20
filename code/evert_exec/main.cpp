
#include <iostream>

#include "MBLib/CFG/CFG.hpp"


using namespace std;


/* [bake me]

dependencies["build_objects"] = [
	"@MBLib>>headers"
]

executable = "main"

[stop baking] */

int main() {
	s_CFG s("noname", {"A", "B", "C"}, "noname", {"1", "0"});
	
	cout << s.Variables()->get_name(0).value;
	
}
