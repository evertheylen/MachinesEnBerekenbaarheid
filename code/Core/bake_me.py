
# separate file because this might get complex

import platform
import os.path

dependencies["build_exec"] = [
    "NGLib/replacor>>build_objects",
    "NGLib/generator>>build_objects",
    "NGLib/outputter>>build_objects",
    "NGLib/python_out>>build_objects"
]



if platform.system() != 'Darwin':
    print("I'm not a Mac, yay!")
    dependencies["build_exec"] += [
       # "libs/boost>make>build",
       # "libs/python3>make>all"
    ]

    # extra config just for this unit
    # #BakerCanHandleIt
    gcc_config = {
        "pre_extra": "-Wno-deprecated -I/usr/include/%s/ -Ilibs/boost/b2stuff/include/"%project_config["python_lib"],
        "post_extra": "-Wl,-Bstatic -Llibs/boost/b2stuff/lib/ -lboost_python -Wl,-Bdynamic -l%s -lutil -ldl -lm"%project_config["python_lib"]
    }
    
else:
    print("I'm a Mac :(")
    
executable = "core"
