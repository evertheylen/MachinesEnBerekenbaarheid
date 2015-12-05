
# separate file because this might get complex

import platform

dependencies["headers"] = [
    "NGLib/outputter>>headers",
]


if platform.system() == 'Darwin':
    # extra config just for this unit
    # #BakerCanHandleIt
    gcc_config = {
        "pre_extra": "-Wno-deprecated -Ilibs/python3/Include -Ilibs/boost/b2stuff/include/",
        #"post_extra": "-pthread -Wl,-Bdynamic -lutil -ldl -lm -Wl,-Bstatic -Llibs/python3 -lpython3.5m -Llibs/boost/b2stuff/lib/ -lboost_python3 -Wl,-Bdynamic"
    }

