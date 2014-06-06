#!/bin/sh

# this script will initialize the submodules of the contrib system,
# update them and build them.

# init/update Git submodules
git submodule init
git submodule update

# build babeltrace
cd babeltrace
./bootstrap
./configure
make
cd ..

# build libdelorean
cd libdelorean
scons
cd ..
