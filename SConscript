Import('RTT_ROOT')
Import('rtconfig')
from building import *

cwd     = GetCurrentDir()
src     = []
CPPPATH = [cwd + '/include']

# add hashmap
src = src + Glob('src/container/hashmap/*.c')
# add queue
src = src + Glob('src/container/queue/*.c')
# add string
src = src + Glob('src/libc/*.c')

group = DefineGroup('cBOX', src, depend = ['PKG_USING_CBOX'], CPPPATH = CPPPATH)

Return('group')
