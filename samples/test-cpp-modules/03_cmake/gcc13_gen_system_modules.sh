#!/usr/bin/env bash

dirname=$(dirname "${CXX}")
# this needs to be a real path, and not dotted forward and backward
CXX_INCLUDE=$(realpath ${dirname}/../include/c++/13)
echo $dirname
echo $CXX_INCLUDE

rm -rf gcm.cache
${CXX} -std=c++20 -xc++-system-header -fmodule-header ${CXX_INCLUDE}/iostream
${CXX} -std=c++20 -xc++-system-header -fmodule-header ${CXX_INCLUDE}/vector
${CXX} -std=c++20 -xc++-system-header -fmodule-header ${CXX_INCLUDE}/numeric
find gcm.cache