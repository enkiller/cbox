# for module compiling
Import('RTT_ROOT')
Import('rtconfig')
from building import *
import os, fnmatch

cwd     = GetCurrentDir()
src     = []
CPPPATH = [cwd + '/include']

def add_all_source_file(path):
    for root, dirs, files in os.walk(path):
        for file in files:
            if fnmatch.fnmatchcase(file, '*.c') or fnmatch.fnmatchcase(file, '*.cpp'):
                yield file

# Add all source files in src directory
src = add_all_source_file('src')

group = DefineGroup('cBOX', src, depend = ['PKG_USING_CBOX'], CPPPATH = CPPPATH)

Return('group')
