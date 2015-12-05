
# separate file because this might get complex

import platform

dependencies["build_exec"] = [
    "NGLib/replacor>>build_objects",
    "NGLib/generator>>build_objects",
    "NGLib/outputter>>build_objects",
    "NGLib/python_out>>build_objects"
]

if platform.system() != 'Darwin':
    print("I'm not a Mac, yay!")
    dependencies["build_exec"] += [
        "libs/boost>make>build",
        "libs/python3>make>all"
    ]

    # extra config just for this unit
    # #BakerCanHandleIt
    gcc_config = {
        "pre_extra": "-Wno-deprecated -Ilibs/python3/Include -Ilibs/boost/b2stuff/include/",
        "post_extra": "-pthread -Wl,-Bstatic -Llibs/python3 -lpython3.4m -Llibs/boost/b2stuff/lib/ -lboost_python3 -Wl,-Bdynamic -lutil -ldl -lm"
    }
    
else:
    print("I'm a Mac :(")
    
executable = "core"
