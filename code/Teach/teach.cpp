
/* [bake me]

dependencies["build_exec"] = [
    "NGLib/replacor>>build_objects",
    "NGLib/generator>>build_objects",
    "NGLib/outputter>>build_objects",
    "NGLib/python_out>>build_objects",
    "MBLib/common>>build_objects",
    "NGLib/teacher>>headers",
    "NGLib/teacher>>build_objects",
]

gcc_config = {
    "pre_extra": "-Wno-deprecated -I%s -Ilibs/boost/b2stuff/include/"%project_config["python_include_dir"],
    "post_extra": "-Wl,-Bstatic -Llibs/boost/b2stuff/lib/ -lboost_python -Wl,-Bdynamic -L{python_lib_dir} -l{python_lib_name} -lutil -ldl -lm".format(**project_config)
}


executable = "teach"

[stop baking] */

#include <memory>
#include <exception>
#include <random>

#include "boost/python.hpp"

#include "NGLib/generator/generator.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/python_out/python_outputter.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "MBLib/common/common.hpp"
#include "NGLib/teacher/teacher.hpp"

int main() {
	// load xml, construct a Teacher
	
	// while not exited
		// create a tree (tree = Teacher.generate(...))
		// display it to the user (Teacher.output(tree, out))
		// input score, use Teacher.score method
	
	std::cout << "hello world\n";
}
