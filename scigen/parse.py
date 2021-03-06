#!/bin/python3

import re
import random

class Var(str):
    pass

# regular expressions here
#p = re.compile("...")
# use:
#p.match("...")

varname = re.compile("([A-Z_0-9]{3,})")
varname_line = re.compile("([A-Z_0-9]{3,})(?:\\+[0-9]*)?\\!?\\s*(.*)")

open_brace = re.compile("([A-Z_0-9]{3,})(?:\\+[0-9]*)?\\!?\\s*\\{\\s*$")
close_brace = re.compile("^\\s*\\}\\s*$")

def get_s(text):
    s = []
    v = varname.search(text)
    while v is not None:
        #print("match", repr(v))
        pre = text[:v.span()[0]]
        found = text[v.span()[0]:v.span()[1]]
        s.append(pre)
        if found == pre:
            s.append(found)
        else:
            s.append(Var(found))
        text = text[v.span()[1]:]
        v = varname.search(text)
    # done searching for variables
    s.append(text)
    return [i for i in s if i!=""]

def add_cfg(cfg, var, val):
    if val != []: # TODO? empty string?
        if var in cfg:
            cfg[var].append(s)
        else:
            cfg[var] = [s]

CFG = {}

f = open("scirules.txt")
raw_lines = f.readlines()
f.close()

lines = []

for l in raw_lines:
    if (not l.startswith("#")) and (l != "") and (l != "\n"):
        lines.append(l[:-1])

try:
    it = iter(lines)
    while True:
        l = next(it)
        m = open_brace.match(l)
        if m is not None:
            var = m.group(1)
            total = "" #"{\n" ?
            l = next(it)
            while close_brace.match(l) is None:
                total += l + "\n"
                l = next(it)
            #total += "\n}\n" ?
            s = get_s(total)
            add_cfg(CFG, var, s)
        else:
            m = varname_line.match(l)
            if m is not None:
                var = m.group(1)
                text = m.group(2)
                #print("text is", text)
                s = get_s(text)
                add_cfg(CFG, var, s)
            else:
                print("no match for ", repr(l))
            
except StopIteration:
    pass


def print_xml():
    print("<Variables>")
    for var in CFG:
        print("[" + var + "]", end="")
    print("</Variables>\n")
    
    print("<Terminals></Terminals>\n")
    
    print("<Productions>")
    ID = 0
    for (k, l) in CFG.items():
        for v in l:
            ID += 1
            print("<Rule LHS=\"[{var}]\" RHS=\"{string}\" ID=\"{ID}\"/>".format(var=k, ID=ID,
                    string="".join(["[" + i + "]" if i in CFG else i for i in v]).replace('"', "&quot;")))
    print("</Productions>\n")

print_xml()

def generate(var):
    print(var)
    if not isinstance(var, Var) or var not in CFG:
        yield var
        return
    
    rule = random.choice(CFG[str(var)])
    for i in rule:
        yield from generate(i)
        
