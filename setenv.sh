#!/bin/sh

if [ -z "$1" ]; then
    echo "please provide the path to libdelorean's root as the first argument"
else
    export LIBDELOREAN_CPPPATH="$1/include"
    export LIBDELOREAN_LIBPATH="$1/src"
    export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$1/src:$(pwd)/src/common"
fi
