
extensions = [".cpp", ".hpp", ".h", ".test"]

collections = {
    "all": ["MBLib"]
}

gcc_config = {
    "mode": "g",
    "std": "c++11",
    "extra": "-Wall"
}

maintenance_config = {
    "dirty_files": ["*.o"]
}

print_commands = False

