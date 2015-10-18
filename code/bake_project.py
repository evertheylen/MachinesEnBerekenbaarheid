
collections = ConfigDict({
    "@execs": ["MBExec"],
    "@all": ["@MBLib", "MBExec"]
})

extensions =  [".cpp", ".cc", ".hpp", ".hh", ".h", ".test"]

print_commands = False

gcc_config = ConfigDict({
    "mode": "g3",
    "std": "c++11",
    "extra": "-Wall",
    "input": ["*.cpp", "*.cc", "*.hpp", "*.h", "*.hh"]
})

maintenance_config = ConfigDict({
    "dirty_files": ["*.o"]
})
