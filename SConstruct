# master SConstruct for tigerbeetle
#
# author: Philippe Proulx <eepp.ca>


import os


# common C++ flags
ccflags = [
    '-std=c++11',
    '-Wall',
    '-g',
]

# this is to allow colorgcc
custom_env = {
    'PATH': os.environ['PATH'],
    'TERM': os.environ['TERM'],
    'HOME': os.environ['HOME'],
}

root_env = Environment(CCFLAGS=ccflags,
                       ENV=custom_env)
if 'CXX' in os.environ:
    root_env['CXX'] = os.environ['CXX']

Export('root_env')

apps = SConscript(os.path.join('src', 'SConscript'))

