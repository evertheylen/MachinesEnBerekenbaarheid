
# separate file because this might get complex

import platform
import os.path

dependencies["build_exec"] = [
    "NGLib/replacor>>build_objects",
    "NGLib/generator>>build_objects",
    "NGLib/outputter>>build_objects",
    "NGLib/python_out>>build_objects",
    "MBLib/common>>build_objects",
    "NGLib/teacher>>build_objects",
]


# extra config just for this unit
# #BakerCanHandleIt
gcc_config = {
    "pre_extra": "-Wno-deprecated -I%s -Ilibs/boost/b2stuff/include/"%project_config["python_include_dir"],
    "post_extra": "-Wl,-Bstatic -Llibs/boost/b2stuff/lib/ -lboost_python -Wl,-Bdynamic -L{python_lib_dir} -l{python_lib_name} -lutil -ldl -lm".format(**project_config)
}

    
executable = "core"
