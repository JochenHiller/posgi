#!/usr/bin/env bash

usage() {
  cat <<EOF
Usage: $0 [clean|fmt|all|test|fuzztest-run|fuzztest-report|lint|help]

  clean            cleanup all generated files
  fmt              format all source files (.cc, .h, CMakelists.txt)
  all              build all targets
  test             run tests
  fuzztest-run     run fuzz tests, might run indefinitely
  fuzztest-report  run coverage report for fuzz tests
  lint             run C++ linter(s) on all source files
  help             this help
EOF
}

# parse args
DO_CLEAN="false"
DO_FMT="false"
DO_ALL="false"
DO_TEST="false"
DO_FUZZ_TEST_RUN="false"
DO_FUZZ_TEST_REPORT="false"
DO_LINT="false"
DO_HELP="false"
for arg in $@ ; do
  if [ "${arg}" = "clean" ] ; then
    DO_CLEAN="true"
  elif [ "${arg}" = "fmt" ] ; then
    DO_FMT="true"
  elif [ "${arg}" = "all" ] ; then
    DO_ALL="true"
  elif [ "${arg}" = "test" ] ; then
    DO_TEST="true"
  elif [ "${arg}" = "fuzztest-run" ] ; then
    DO_FUZZ_TEST_RUN="true"
  elif [ "${arg}" = "fuzztest-report" ] ; then
    DO_FUZZ_TEST_REPORT="true"
  elif [ "${arg}" = "lint" ] ; then
    DO_LINT="true"
  elif [ "${arg}" = "help" ] ; then
    DO_HELP="true"
  else
    echo "WARN: Unknown command ${arg}"
    DO_HELP="true"
  fi
done

if    [ "${DO_CLEAN}" = "false" ] \
   && [ "${DO_FMT}" = "false" ]   \
   && [ "${DO_ALL}" = "false" ]   \
   && [ "${DO_TEST}" = "false" ]  \
   && [ "${DO_FUZZ_TEST_RUN}" = "false" ]  \
   && [ "${DO_FUZZ_TEST_REPORT}" = "false" ]  \
   && [ "${DO_LINT}" = "false" ]  \
   ; then
  DO_HELP="true"
fi

if [ "${DO_HELP}" = "true" ] ; then
  usage
  exit 1
fi

if [ "${DO_CLEAN}" = "true" ] ; then
  if [ -d build ] ; then
    echo "Clean ./build directory"
    rm -rf ./build
  fi
  rm -fv lint-*.log posgi.log */posgi.log */*/posgi.log
fi

if [ "${DO_FMT}" = "true" ] ; then
  # see https://github.com/zemasoft/clangformat-cmake
  # Option A)
  # cmake --build build --target clangformat
  # Option B)
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
  (cd build ; make clangformat)
  echo "Format CMakeLists.txt"
  cmake-format -i CMakeLists.txt
fi

if [ "${DO_ALL}" = "true" ] ; then
  echo "Make ./build directory"
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
  (cd build ; make)
  ls -al build/libposgi* build/posgi*
fi

if [ "${DO_TEST}" = "true" ] ; then
  echo "Run tests"
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
  cmake --build build
  (cd build ; ctest --output-on-failure)
  # show console output when running tests. Useful during development
  # (cd build ; ctest --output-on-failure --gtest_catch_exceptions=0)
fi

if [ "${DO_FUZZ_TEST_RUN}" = "true" ] ; then
  echo "Run fuzz tests"
  cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
  # we start with some good inputs to give fuzzer a good start
  # TODO(JochenHiller): what to do if we have more fuzz tests?
  ./third_party/cifuzz/bin/cifuzz run --interactive=false \
    --seed-corpus framework/impl/posgi_fuzz_tests_inputs posgi_fuzz_tests
fi

if [ "${DO_FUZZ_TEST_REPORT}" = "true" ] ; then
  echo "Report fuzz tests coverage"
  ./third_party/cifuzz/bin/cifuzz coverage --output=./build/fuzz-test-coverage-report posgi_fuzz_tests
fi

# TODO(JochenHiller): lint-cpplint, lint-clang-tidy, lint-iwyu
if [ "${DO_LINT}" = "true" ] ; then
  # see https://stackoverflow.com/questions/51582604/how-to-use-cpplint-code-style-checking-with-cmake
  which cpplint >/dev/null
  if [ $? = 0 ] ; then
    rm -rf ./build ; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      "-DCMAKE_CXX_CPPLINT=cpplint;--verbose=0;--quiet" -S . -B build
    (cd build ; make clean all) 2>&1 | tee lint-cpplint.log
    :
  else
    echo "WARN: Could not find cpplint, ignoring..."
  fi

  # TODO(JochenHiller): does not run on Linux
  which clang-tidy >/dev/null
  if [ $? = 0 ] ; then
    rm -rf ./build ; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      "-DCMAKE_CXX_CLANG_TIDY=clang-tidy" -S . -B build
    (cd build ; make clean all) 2>&1 | tee lint-clang-tidy.log
  else
    echo "WARN: Could not find clang-tidy, ignoring..."
  fi

  # see https://include-what-you-use.org/. This linter does not really give useful results, therefoe verbose=0
  which include-what-you-use >/dev/null
  if [ $? = 0 ] ; then
    # add -Xiwyu; before every iwyu flag
    rm -rf ./build ; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      "-DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=include-what-you-use;-w;-Xiwyu;--no_fwd_decls;-Xiwyu;--verbose=0" -S . -B build
    (cd build ; make clean all) 2>&1 | tee lint-iwyu.log
    :
  else
    echo "WARN: Could not find include-what-you-use, ignoring..."
  fi
fi
