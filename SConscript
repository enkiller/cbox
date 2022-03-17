Import('RTT_ROOT')
Import('rtconfig')
from building import *

cwd     = GetCurrentDir()
src     = []
CPPPATH = [cwd + '/include']

# add hashmap
src = src + Glob('src/container/hashmap/*.c')
# add string
src = src + Glob('src/libc/*.c')

# add test
if GetDepend('PKG_USING_CBOX_TEST'):
    src = src + Glob('test/*.c')
    src = src + ['rtthread_tc.c']

group = DefineGroup('cBOX', src, depend = ['PKG_USING_CBOX'], CPPPATH = CPPPATH)

Return('group')
