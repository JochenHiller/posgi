#!/usr/bin/env bash

usage() {
  cat <<EOF
Usage: $0 [clean|fmt|all|lint]

  clean    will cleanup all generated files
  fmt      will format all source files (.cc, .h, CMakelists.txt)
  all      will build all targets
  lint     will run C++ linter(s) on all source files
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
    rm -f posgi.log lint-*.log
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

# TODO(JochenHiller): lint-cpplint, lint-clang-tidy, lint-iwyu
if [ "${1}" = "lint" ] ; then
  # see https://stackoverflow.com/questions/51582604/how-to-use-cpplint-code-style-checking-with-cmake
  which cpplint >/dev/null
  if [ $? = 0 ] ; then
    rm -rf ./build ; cmake "-DCMAKE_CXX_CPPLINT=cpplint;--verbose=0;--quiet" -S . -B build
    (cd build ; make clean all) 2>&1 | tee lint-cpplint.log
    :
  else
    echo "WARN: Could not find cpplint, ignoring..."
  fi
  which clang-tidy >/dev/null
  if [ $? = 0 ] ; then
    pwd=$(PWD)
    rm -rf ./build ; cmake "-DCMAKE_CXX_CLANG_TIDY=clang-tidy" -S . -B build
    (cd build ; make clean all) 2>&1 | tee lint-clang-tidy.log
  else
    echo "WARN: Could not find clang-tidy, ignoring..."
  fi
  which include-what-you-use >/dev/null
  if [ $? = 0 ] ; then
    # rm -rf ./build ; cmake "-DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=include-what-you-use;--transitive_includes_only;-w;-Xiwyu;--verbose=7" -S . -B build
    # (cd build ; make clean all) 2>&1 | tee lint-iwyu.log
    :
  else
    echo "WARN: Could not find include-what-you-use, ignoring..."
  fi
  shift
fi
