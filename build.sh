#!/usr/bin/env bash

if [ "${1}" = "clean" ] ; then
  if [ -d build ] ; then
    echo "Clean ./build directory"
    rm -rf ./build
  fi
  shift
fi

if [ "${1}" = "all" ] ; then
  echo "Make ./build directory"
  cmake -S . -B build
  (cd build ; make)
  ls -al build/libposgi* build/posgi*
  shift
fi
