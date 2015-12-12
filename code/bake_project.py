
collections = ConfigDict({
    "@execs": ["MBExec", "Core"],
    "@all": ["@MBLib", "MBExec", "@NGLib", "NextGen", "Core", "libs/tinyxml"]
})

extensions =  [".cpp", ".cc", ".hpp", ".hh", ".h", ".test"]

scanned_files =  ["*.cpp", "*.cc", "*.hpp", "*.hh", "*.h", "*.test"]

gcc_config = ConfigDict({
    "compiler": "g++",
    "mode": "g3",
    "std": "c++11",
    "pre_extra": "-Wall",
    "post_extra": "",
    "input": ["*.cpp", "*.cc", "*.hpp", "*.h", "*.hh"]
})

maintenance_config = ConfigDict({
    "dirty_files": ["*.o", "*.tcpp", "*.to", "*.bin", "*.headers_baked"]
})

gtest_config = ConfigDict({
    "location": "libs/gtest",
    "static_libs": "libs/gtest/cmake_stuff",
    "template": """

// Automatically generated by Baker
#include "gtest/gtest.h"

{tests}

int main(int argc, char **argv) {{
    ::testing::InitGoogleTest(&argc, argv);
    // Get rid of the irritating threading warning when testing DEATH
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    return RUN_ALL_TESTS();
}}

"""
})

import platform

def find_first(l):
    for f in l:
        if os.path.isfile(f) or os.path.islink(f):
            return f

def libname(f):
    f = os.path.basename(f)
    if f.startswith("lib"):
        f = f[3:]
    if f.find(".a") != -1:
        f = f[:f.find(".a")]
    if f.find(".so") != -1:
        f = f[:f.find(".so")]
    if f.find(".dylib") != -1:
        f = f[:f.find(".dylib")]
    return f

python_lib_locs = ["/usr/lib/libpython3.5m.so", "/usr/lib/libpython3.4m.so", "/usr/lib/x86_64-linux-gnu/libpython3.5m.so.1", "/usr/lib/x86_64-linux-gnu/libpython3.4m.so.1",
                   "/Library/Frameworks/Python.framework/Versions/3.4/lib/libpython3.4m.dylib", "/Library/Frameworks/Python.framework/Versions/3.5/lib/libpython3.5m.dylib"]

python_lib_dir = os.path.dirname(find_first(python_lib_locs))
python_lib_name = libname(find_first(python_lib_locs))
print("Found python in", python_lib_dir)


python_include_dir = os.path.dirname(find_first(["/usr/include/%s/Python.h"%libname(python_lib_name),
            "/Library/Frameworks/Python.framework/Versions/3.4/Include/python3.4/Python.h", "/Library/Frameworks/Python.framework/Versions/3.5/Include/Python.h"]))

