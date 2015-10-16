#!/usr/bin/python

import subprocess # call
import shlex  # splitting for call

import os  # listdir, path, getcwd
import argparse
import glob  # glob.glob

import pdb  # debugging, pdb.set_trace()

# bake_me scripts can put global data in here
global global_data
global_data = {
    "collections": [],
    "extensions": [],
    "print_commands": False,
}

global all_units
# key is location!!
all_units = {}

global all_todos
# for a given todo t: all_todos[t] = t
all_todos = {}
# I agree this is weird. It's basically like a set only with an easier way
# to retrieve the value already in there, so we don't create the same Todo
# (as defined by __eq__) two times (different objects in memory)



# ----------------------------------------------------
# COLORS YAY
endc = '\033[0m'  # Resets all ANSI attributes
 
red = (255, 0, 0)
green = (0, 255, 0)
blue = (0, 0, 255)
 
cyan = (0, 255, 255)
pink = (255, 0, 255)
yellow = (255, 255, 0)

orange = (255, 165, 0)

black = (0, 0, 0)
white = (255, 255, 255)
 
def _rgb(rf,gf,bf,rb=0,gb=0,bb=0):
    # red front, ..., blue back
    return "\033[1;38;2;{};{};{};48;2;{};{};{}m".format(rf,gf,bf,rb,gb,bb)

def rgb(f, b=black):
    return _rgb(f[0], f[1], f[2], b[0], b[1], b[2])

def rgbtext(s, f=red, b=black):
    return rgb(f,b) + s + endc

# -------------------------------------------------------
# Logging

class IndentWriter:
    def __init__(self):
        self.indent = 0
        self.byte_cache = []
        
    def writeline(self, l):
        self.byte_cache.clear()
        print(self.indent*"  " + l)
    
    def writebytes(self, b):
        s = b.decode("utf-8")[:-1]
        if s.isspace() or s == "":
            self.byte_cache.append(s)
        else:
            for cs in self.byte_cache:
                print(self.indent*"  " + cs)
            self.byte_cache.clear()
            print(self.indent*"  " + s)
    
    def tab(self):
        # max indent of 10 (20 spaces)
        self.indent = min(10, self.indent+1)
    
    def untab(self):
        self.indent = max(0, self.indent-1)



# ------------------------------------------------------
# Exceptions

class BakeError(Exception):
    pass

class ConfigError(BakeError):
    def __init__(self, where, key, info=""):
        self.where = where
        self.key = key
        self.info = info
    
    def __str__(self):
        return rgbtext(("Error in configuration: '{s.where}' should contain a proper value for '{s.key}'." + (" ("+self.info+")" if self.info != "" else "")).format(s=self), orange)

class BuildError(BakeError):
    def __init__(self, todo, info=""):
        self.todo = todo
        self.info = info
        
    def __str__(self):
        return rgbtext(("Error while building todo '{s.todo}'." + (" ("+self.info+")" if self.info != "" else "")).format(s=self), red)



# ------------------------------------------------------
# Utilities

def call(s, writer):
    if global_data["print_commands"]:
        writer.writeline(">> " + s)
    # if shell=True, it is recommended to pass the command as a string, rather than as a list
    p = subprocess.Popen(s, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    while True:
        retcode = p.poll()
        writer.writebytes(p.stdout.readline())
        # stderr??
        if retcode is not None:
            for l in p.stdout.readlines():
                writer.writebytes(l)
            return retcode

def check_call(s, writer, todo, info=""):
    ret = call(s, writer)
    if ret != 0:
        raise BuildError(todo, info)
    
    
def drop_ext(s):
    return s[:s.rfind(".")]

def there_exists(collection, condition):
    for i in collection:
        if condition(i):
            return True
    return False

# returns None if key not in dct, dct[key] otherwise
def get(key, dct):
    if key in dct:
        return dct[key]
    return None

def empty(e):
    return (e is None or e == "")

def pick_nonempty(*args):
    for i in args:
        if not empty(i):
            return i

def exec_file(location, loc=locals()):
    #print("searching ",location)
    if os.path.isfile(location):
        f = open(location)
        fc = compile(f.read(), location.split("/")[-1], "exec")
        exec(fc, globals(), loc)
        return True
    return False




# -----------------------------------------------------------
# Units

def create_unit(uname):
    uloc = os.path.abspath(uname)
    if uloc in all_units:
        return all_units[uloc]
    else:
        u = Unit(uname)
        u.scan()
        all_units[u.location] = u
        return u

# not an actual unit
class BaseUnit:
    def __init__(self, name):
        self.name = name

class Unit(BaseUnit):
    def __init__(self, name):
        self.name = name
        self.location = os.path.abspath(name)
        # TODO set sensible defaults
        self.data = {
            "worker": "",
            "dependencies": {},
        }
        self.worker = None
        self.deps = {}  # for each action, certain deps
        # worker and deps have special meaning
        
        self.files = None
    
    def __eq__(self, other):
        return os.path.samefile(self.location, other.location)
    
    def __hash__(self):
        return hash(self.location)
    
    def scan(self):
        # checks all files for baker-related stuff
        # these files are either bake_me.py or marked comments
        # within sources with the proper extension
        self.files = []
        
        # first, bake_me.py
        bake_me_loc = os.path.join(self.location, "bake_me.py")
        try:
            if exec_file(bake_me_loc, self.data):
                self.files.append(bake_me_loc)
        except Exception as e:
            raise ConfigError(bake_me_loc, "whatever", str(e))
            #print("hello is:", global_data["hello"] if "hello" in global_data else "?????")
        
        # then, check all files
        for fname in os.listdir(self.location):
            if there_exists(global_data["extensions"], lambda e: fname.endswith(e)):
                f_loc = os.path.join(self.location, fname)
                self.files.append(f_loc)
                f = open(f_loc)
                bake_code = ""
                copy = False
                for line in f:
                    if "[bake me]" in line:
                        copy = True
                    elif "[stop baking]" in line:
                        copy = False
                    elif copy:
                        bake_code += line + "\n"
                
                try:
                    exec(bake_code, globals(), self.data)
                except Exception as e:
                    raise ConfigError(floc, "[bake me]", str(e))
                
                f.close()
        
        
        self.worker = workers[self.data["worker"]]
        # TODO check for legit action?
        for action in self.data["dependencies"]:
            self.deps[action] = set()
            for dep in self.data["dependencies"][action]:
                # format (regex): "unit>(worker)?>(action)?"
                parts = dep.split(">")
                assert len(parts) == 3  # TODO shouldn't assert, raise exception instead
                dname, dworker_s, daction = tuple(parts)
                dunit = create_unit(dname)
                dworker = workers[dworker_s]  # this can be None, but should be fixed by the Todo's
                # daction remains daction
                self.deps[action].add(Dependency(dunit, dworker, daction))
    
    def list_deps(self):
        print("\nUnit "+self.name)
        for action in self.deps:
            print("  - Action "+action)
            for dep in self.deps[action]:
                print("     - " + str(dep))
       
       


# -------------------------------------------------------------------------------
# Dependencies and Todo's

class Dependency:
    def __init__(self, unit, worker, action):
        self.unit = unit
        self.worker = pick_nonempty(worker, unit.worker)
        self.action = action
    
    def __str__(self):
        return self.unit.name + ">" + (str(self.worker.shortname) if self.worker is not None else "") + ">" + self.action
    
    __repr__ = __str__
    
    def __eq__(self, other):
        # not comparing self.deps, assuming that given the other 3, deps is already equal
        return (self.unit, self.worker, self.action) == (other.unit, other.worker, other.action)
    
    def __hash__(self):
        return hash((self.unit, self.worker, self.action))


class Todo(Dependency):
    def __init__(self, dep):
        self.unit = dep.unit
        self.worker = dep.worker
        self.action = dep.action
        
        self.done = False
        
        # a Dependency may contain null values or empty string for worker/action...
        self.deps = set()
        if self.action in self.unit.deps:
            self.build_deps(self.unit.deps[self.action])
        
        # also allow extra deps based on the worker and action
        self.build_deps(self.worker.extra_deps(self))        
    
    def build_deps(self, deps):
        for d in deps:
            self.deps.add(pick_nonempty_todo(d, self))
    
    def all_deps(self):
        yield self
        for d in self.deps:
            yield from d.all_deps()
    
    def list_deps(self, writer):
        writer.writeline(indent*"  " + "- " + str(self))
        writer.tab()
        for d in self.deps:
            d.list_deps(writer)
        writer.untab()
    
    def do(self, writer):
        if self.done:
            writer.writeline(rgbtext("Already done '" + str(self)+"'", green))
            return
        if len(self.deps) > 0:
            writer.writeline(rgbtext("Dependencies of Todo '" + str(self)+"':", cyan))
            writer.tab()
            for d in self.deps:
                d.do(writer)
            writer.untab()
        writer.writeline(rgbtext("Performing Todo '" + str(self)+"'", cyan))
        self.worker.do(self, writer)
        self.done = True


# creates only unique todos
def create_todo(dep):
    t = Todo(dep)
    if t in all_todos:
        return all_todos[t]
    all_todos[t] = t
    return t


def passthrough(u):
    if len(u.files) == 0:
        subs = set()
        # 'passthrough', all subfolders with actual files are selected
        for d in os.listdir(u.location):
            if os.path.isdir(d):
                subu = create_unit(d)
                subs.update(passthrough(subu))
    else:
        return set([u])

        
def pick_nonempty_todo(high, low):  # ... priority
    # worker and action of low may be taken if high is empty for those attributes
    # high must always have a unit ofc
    return create_todo(Dependency(high.unit, pick_nonempty(high.worker, low.worker), pick_nonempty(high.action, low.action)))


class RootTodo(Todo):
    def __init__(self, worker, action, cmd_units):
        self.unit = BaseUnit(project_name)
        self.worker = worker
        self.action = action
        self.deps = set()
        self.build_deps([Dependency(u, None, "") for u in cmd_units])
    
    def do(self, writer):
        for d in self.deps:
            try:
                d.do(writer)
            except BuildError as e:
                writer.indent = 0
                writer.writeline(str(e))
            writer.writeline("")
        
        writer.writeline("All done!")






# ---------------------------------------------------------------------------------------
# Workers

# TODO more compiler/worker support may come
class Worker:
    shortname = "LAZY DEVELOPER DETECTED"  # aka override me pls
    
    def extra_deps(self, todo):
        # extra dependencies implicitly given by the action
        return set([])  # by returns nothing
    
    def do(self, todo):
        # by default does nothing
        pass


class EasyWorker(Worker):
    def do(self, todo, writer):
        if todo.action in self.actions:
            self.actions[todo.action](self, todo, writer)
        else:
            writer.writeline("Ignoring action %s on %s"%(todo.action,todo.unit.name))


# --- Actual workers ---

class GccCompiler(EasyWorker):
    shortname = "gcc"
    
    def __init__(self, config):
        self.__dict__.update(config)
        self.cmd_object = "g++ -{s.mode} -std={s.std} -c {source} -o {objloc} {include} {s.extra}"
        self.cmd_exec = "g++ -{s.mode} -std={s.std} -o {execloc} {objects} {s.extra}"
    
    def extra_deps(self, todo):
        extra = set()
        if todo.action == "build_exec":
            extra.add(Dependency(todo.unit, todo.worker, "build_objects"))
            for d in todo.deps:
                if d.action == "build_objects":
                    for subd in d.all_deps():
                        if subd.action == "headers":
                            extra.add(Dependency(subd.unit, subd.worker, "build_objects"))
        return extra
    
    def build_objects(self, todo, writer):
        includes = set([os.getcwd()])
        for d in todo.all_deps():
            if d.action == "headers":
                includes.add(d.unit.location)
        
        include = " ".join(["-I %s/"%i for i in includes])
        
        # typical gcc call for objects:
        #  g++ -O3 -std=c++11 -I /include/me -c obj.cpp
        #  --> produces obj.o
        
        for source in glob.glob(os.path.join(todo.unit.location, "*.cpp")):
            filename = drop_ext(source)
            objloc = os.path.join(todo.unit.location, filename+".o")
            check_call(self.cmd_object.format(s=self, source=source, objloc=objloc, include=include), writer, todo)
        
    def build_exec(self, todo, writer):
        # typical gcc call for executables:
        #  g++ -O3 -std=c++11 -I /include/me -o exec obj.o obj2.o obj3.o
        #  --> produces exec
        
        executable = todo.unit.data["executable"] if "executable" in todo.unit.data else None
        if executable is not None:
            objects = set(glob.glob(os.path.join(todo.unit.location, "*.o")))
            for d in todo.all_deps():
                if d.action == "build_objects":
                    # dependencies should've already been built
                    objects.update(glob.glob(os.path.join(d.unit.location, "*.o")))
            
            objects = " ".join(objects)
            execloc = os.path.join(todo.unit.location, executable)
            check_call(self.cmd_exec.format(s=self, execloc=execloc, objects=objects), writer, todo)
        else:
            raise ConfigError(todo.unit.name, "executable", "name of the resulting executable")
    
    actions = {
        "build_objects": build_objects,
        "build_exec": build_exec,
    }


class Maintenance(EasyWorker):
    shortname = "maintenance"
    
    def __init__(self, config):
        if "dirty_files" not in config:
            raise ConfigError("bake_project.py", "dirty_files", "a list of globs, eg. ['*.o', '~*']")
        self.__dict__.update(config)
    
    def clean(self, todo, writer):
        for regex in self.dirty_files:
            for l in glob.glob(os.path.join(todo.unit.location, regex)):
                os.remove(l)
    
    def clean_all(self, todo, writer):
        self.clean(todo, writer)
        exe = get("executable", todo.unit.data)
        if exe is not None:
            loc = os.path.join(todo.unit.location, exe)
            if os.path.isfile(loc):
                os.remove(loc)
    
    actions = {
        "clean": clean,
        "clean_all": clean_all  # also clean executables
    }




# ------------------------------------------------------------------------------------
# Main and config stuff

to_be_configured_workers = {
    "": None,  # default
    "gcc": (GccCompiler, "gcc_config"),
    "maintenance": (Maintenance, "maintenance_config"),
    "gtester": Worker,
}

global workers
workers = {}

# if none is specified on cmd
standard_workers_by_action = {
    "build_exec": "gcc",
    "build_objects": "gcc",
    "clean": "maintenance",
    "clean_all": "maintenance",
}

global project_name
project_name = os.path.split(os.getcwd())[-1]


def main():
    # which unit to bui-- perform some action on?
    
    parser = argparse.ArgumentParser(description="A build tool that works.")
    parser.add_argument("action", metavar="A", type=str, help="The action the worker should perform")
    parser.add_argument("units", metavar="U", type=str, nargs="+", help="A unit (given by relative path) to use")
    # TODO other actions
    parser.add_argument("--worker", metavar="C", type=str, help="worker to use", choices=to_be_configured_workers.keys(), default="DEFAULT_BY_ACTION")
    parser.add_argument("--list-deps", action="store_true", help="simply list the dependencies of all units concerned", default=False)
    
    args = parser.parse_args()
    
    writer = IndentWriter()
    
    if not exec_file("bake_project.py", global_data):
        raise ConfigError("this directory", "bake_project.py", "an empty file works too")
    
    # parse workers
    for k, t in to_be_configured_workers.items():
        if isinstance(t, tuple):
            if len(t) == 1:
                workers[k] = t[0]() if t[0] is not None else None
            else:
                try:
                    workers[k] = t[0](global_data[t[1]])
                except KeyError:
                    raise ConfigError("bake_project.py", t[1])
        else:
            workers[k] = t() if t is not None else None
    
    cmd_units = set()
    
    for uname in args.units:
        if uname.startswith("@"):
            colname = uname[1:]
            if colname not in global_data["collections"]:
                u = create_unit(colname)
                cmd_units.add(u)
                pt = passthrough(u)
                cmd_units.update(pt)
            else:
                for sub_uname in global_data["collections"][colname]:
                    subu = create_unit(sub_uname)
                    cmd_units.add(subu)
        else:
            u = create_unit(uname)
            cmd_units.add(u)
    
    
    if args.list_deps:
        for u in all_units.values():
            u.list_deps()
    else:
        # already checked for 'emptiness' in argparser
        default_worker = ""
        if args.worker == "DEFAULT_BY_ACTION":
            default_worker = workers[standard_workers_by_action[args.action]]
        else:
            default_worker = args.worker
        
        default_action = args.action
        
        root = RootTodo(default_worker, default_action, cmd_units)
        root.do(writer)
        #root.list_deps()
        
        

if __name__ == "__main__":
    try:
        main()
    except BakeError as e:
        print(e)
    
