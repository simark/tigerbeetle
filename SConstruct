# master SConstruct for tigerbeetle
#
# author: Philippe Proulx <eepp.ca>


import os


# common C++ flags
ccflags = [
    '-std=c++11',
    '-Wall',
    '-g',
    '-O2',
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

if 'LIBDELOREAN_CPPPATH' in os.environ:
    root_env.Append(CPPPATH=[os.environ['LIBDELOREAN_CPPPATH']])

if 'LIBDELOREAN_LIBPATH' in os.environ:
    root_env.Append(LIBPATH=[os.environ['LIBDELOREAN_LIBPATH']])

if 'BABELTRACE_CPPPATH' in os.environ:
    root_env.Append(CPPPATH=[os.environ['BABELTRACE_CPPPATH']])

if 'BABELTRACE_LIBPATH' in os.environ:
    root_env.Append(LIBPATH=[os.environ['BABELTRACE_LIBPATH']])

if 'BABELTRACE_CTF_LIBPATH' in os.environ:
    root_env.Append(LIBPATH=[os.environ['BABELTRACE_CTF_LIBPATH']])

if 'LD_LIBRARY_PATH' in os.environ:
    root_env['ENV']['LD_LIBRARY_PATH'] = os.environ['LD_LIBRARY_PATH']

Export('root_env')

apps = SConscript(os.path.join('src', 'SConscript'))
