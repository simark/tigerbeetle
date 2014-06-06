#!/bin/bash

# this script will initialize the submodules of the contrib system,
# update them and build them.

# init/update Git submodules
pushd ..
git submodule init
git submodule update
popd

# build babeltrace
pushd babeltrace
./bootstrap
./configure
make
popd

# build libdelorean
pushd libdelorean
scons
popd
