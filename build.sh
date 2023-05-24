#!/usr/bin/env bash

usage() {
  cat <<EOF
Usage: $0 [clean|fmt|all]

  clean    will cleanup all generated files
  fmt      will format all source files (.cc, .h, CMakelists.txt)
  all      will build all targets
EOF
}

if [ $# = 0 ] ; then
  usage
  exit 1
fi

if [ "${1}" = "clean" ] ; then
  if [ -d build ] ; then
    echo "Clean ./build directory"
    rm -rf ./build
  fi
  shift
fi

if [ "${1}" = "fmt" ] ; then
  # see https://github.com/zemasoft/clangformat-cmake
  # Option A)
  # cmake --build build --target clangformat
  # Option B)
  cmake -S . -B build
  (cd build ; make clangformat)
  cmake-format -i CMakeLists.txt
  shift
fi

if [ "${1}" = "all" ] ; then
  echo "Make ./build directory"
  cmake -S . -B build
  (cd build ; make)
  ls -al build/libposgi* build/posgi*
  shift
fi
