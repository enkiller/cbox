# for module compiling
Import('RTT_ROOT')
Import('rtconfig')
from building import *
import os, fnmatch

cwd     = GetCurrentDir()
src     = []
CPPPATH = [cwd + '/include']

def add_all_source_file(path):
    s = []
    for filename in os.listdir(path):
        pathname = os.path.join(path, filename)
        if os.path.isfile(pathname):
            if fnmatch.fnmatchcase(filename, '*.c') or fnmatch.fnmatchcase(filename, '*.cpp'):
                s.append(pathname)
        elif os.path.isdir(pathname):
            s = s + add_all_source_file(pathname)
    return s

# Add all source files in src directory
src = add_all_source_file("src")

group = DefineGroup('cBOX', src, depend = ['PKG_USING_CBOX'], CPPPATH = CPPPATH)

Return('group')
