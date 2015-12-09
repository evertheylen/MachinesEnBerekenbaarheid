
# separate file because this might get complex

import platform

dependencies["headers"] = [
    "NGLib/outputter>>headers",
]



if platform.system() != 'Darwin':
    # extra config just for this unit
    # #BakerCanHandleIt
    gcc_config = {
        "pre_extra": "-Wno-deprecated -I/usr/include/python3.5m/ -Ilibs/boost/b2stuff/include/",
        "post_extra": "-pthread -Wl,-Bdynamic -lutil -ldl -lm -Wl,-Bstatic -Llibs/boost/b2stuff/lib/ -lboost_python3 -Wl,-Bdynamic -lpython3.5m"
    }

