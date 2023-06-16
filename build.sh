#!/usr/bin/env bash

usage() {
  cat <<EOF
Usage: $0 [clean|fmt|compile|test|fuzztest-run-*|fuzztest-report|fuzztest-clean|lint|help|all]

  clean              cleanup all generated files
  fmt                format all source files (.cc, .h, CMakelists.txt)
  compile            build all targets
  test               run tests
  fuzztest-run-*     run fuzz tests, might run indefinitely
    fuzztest-run-01  run fuzz tests for manifest
    fuzztest-run-02  run fuzz tests for PosgiTxtFormatter.formatColumn
  fuzztest-report    run coverage report for fuzz tests
  fuzztest-clean     clean all temporary fuzz tests files
  lint               run all C++ linter(s) on all source files
    lint-cpplint     run cpplint only
    lint-clang-tidy  run clang-tidy only
    lint-iwyu        run include-what-you-use only
  help               this help
  all                will run clean, fmt, compile, test, lint in a row
EOF
}

log() {
  level=$1
  shift
  echo "[build.sh] ${level} $*"
}
log_info() {
  log "INFO" "$*"
}
log_warn() {
  log "WARN" "$*"
}
log_newline() {
  echo " "
}

# parse args
DO_CLEAN="false"
DO_FMT="false"
DO_COMPILE="false"
DO_TEST="false"
DO_FUZZ_TEST_RUN="false"
FUZZ_TEST_RUN_ARG=""
DO_FUZZ_TEST_REPORT="false"
DO_FUZZ_TEST_CLEAN="false"
DO_LINT_CPPLINT="false"
DO_LINT_CLANG_TIDY="false"
DO_LINT_IWYU="false"
DO_HELP="false"

for arg in $@ ; do
  if [ "${arg}" = "clean" ] ; then
    DO_CLEAN="true"
  elif [ "${arg}" = "fmt" ] ; then
    DO_FMT="true"
  elif [ "${arg}" = "compile" ] ; then
    DO_COMPILE="true"
  elif [ "${arg}" = "test" ] ; then
    DO_TEST="true"
  elif [[ ${arg} =~ fuzztest-run* ]] ; then
    DO_FUZZ_TEST_RUN="true"
    FUZZ_TEST_RUN_ARG="${arg}"
  elif [ "${arg}" = "fuzztest-report" ] ; then
    DO_FUZZ_TEST_REPORT="true"
  elif [ "${arg}" = "fuzztest-clean" ] ; then
    DO_FUZZ_TEST_CLEAN="true"
  elif [ "${arg}" = "lint" ] ; then # all linters
    DO_LINT_CPPLINT="true"
    DO_LINT_CLANG_TIDY="true"
    DO_LINT_IWYU="true"
  elif [ "${arg}" = "lint-cpplint" ] ; then
    DO_LINT_CPPLINT="true"
  elif [ "${arg}" = "lint-clang-tidy" ] ; then
    DO_LINT_CLANG_TIDY="true"
  elif [ "${arg}" = "lint-iwyu" ] ; then
    DO_LINT_IWYU="true"
  elif [ "${arg}" = "help" ] ; then
    DO_HELP="true"
  elif [ "${arg}" = "all" ] ; then
    DO_CLEAN="true"
    DO_FMT="true"
    DO_COMPILE="true"
    DO_TEST="true"
    DO_LINT_CPPLINT="true"
    DO_LINT_CLANG_TIDY="true"
    DO_LINT_IWYU="true"
  else
    log_warn "Unknown command ${arg}"
    DO_HELP="true"
  fi
done

if    [ "${DO_CLEAN}" = "false" ] \
   && [ "${DO_FMT}" = "false" ]   \
   && [ "${DO_COMPILE}" = "false" ]   \
   && [ "${DO_TEST}" = "false" ]  \
   && [ "${DO_FUZZ_TEST_RUN}" = "false" ]  \
   && [ "${DO_FUZZ_TEST_REPORT}" = "false" ]  \
   && [ "${DO_LINT_CPPLINT}" = "false" ]  \
   && [ "${DO_LINT_CLANG_TIDY}" = "false" ]  \
   && [ "${DO_LINT_IWYU}" = "false" ]  \
   ; then
  DO_HELP="true"
fi

if [ "${DO_HELP}" = "true" ] ; then
  usage
  exit 1
fi


if [ "${DO_CLEAN}" = "true" ] ; then
  if [ -d build ] ; then
    log_info "Clean all generated files"
    rm -rf ./build
  fi
  rm -fv lint-*.log posgi.log */posgi.log */*/posgi.log
  log_newline
fi

if [ "${DO_FMT}" = "true" ] ; then
  log_info "Format all sources"
  # see https://github.com/zemasoft/clangformat-cmake
  # Option A)
  # cmake --build build --target clangformat
  # Option B)
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
  (cd build ; make clangformat)
  log_info "Format CMakeLists.txt"
  cmake-format -i CMakeLists.txt
  log_newline
fi

if [ "${DO_COMPILE}" = "true" ] ; then
  log_info "Compile project"
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
  (cd build ; make)
  ls -al build/libposgi* build/posgi_cli \
    build/posgi_fuzz_tests_manifest \
    build/posgi_fuzz_tests_txtformatter \
    build/posgi_sample03 build/posgi_tests
  log_newline
fi

if [ "${DO_TEST}" = "true" ] ; then
  log_info "Run all tests"
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
  cmake --build build
  (cd build ; ctest --output-on-failure)
  # below: show console output when running tests. Useful during development
  # (cd build ; ctest --output-on-failure --gtest_catch_exceptions=0)
  log_newline
fi

if [ "${DO_FUZZ_TEST_RUN}" = "true" ] ; then
  log_info "Run fuzz tests"
  cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build

  if [ "${FUZZ_TEST_RUN_ARG}" = "fuzztest-run-01" ] ; then
    log_info "Run fuzz tests for manifest"
    ./third_party/cifuzz/bin/cifuzz run --interactive=false \
      --seed-corpus framework/impl/fuzz_tests_manifest_inputs posgi_fuzz_tests_manifest
    exit 0
  fi
  if [ "${FUZZ_TEST_RUN_ARG}" = "fuzztest-run-02" ] ; then
    log_info "Run fuzz tests for PosgiTxtFormatter.fixed_column"
    ./third_party/cifuzz/bin/cifuzz run --interactive=false \
      --seed-corpus framework/impl/utils/fuzz_tesst_txtformatter_inputs posgi_fuzz_tests_txtformatter
    exit 0
  fi
  log_newline
fi

if [ "${DO_FUZZ_TEST_REPORT}" = "true" ] ; then
  log_info "Report fuzz tests coverage"
  ./third_party/cifuzz/bin/cifuzz coverage --output=./build/fuzz-tests-manifest-coverage-report posgi_fuzz_tests_manifest
  ./third_party/cifuzz/bin/cifuzz coverage --output=./build/fuzz-tests-txtformatter-coverage-report posgi_fuzz_tests_txtformatter
  log_newline
fi

if [ "${DO_FUZZ_TEST_CLEAN}" = "true" ] ; then
  if [ -d ./.cifuzz-build ] || [ -d ./.cifuzz-corpus ] ; then
    log_info "Clean fuzz tests data"
    rm -rf ./.cifuzz-build ./.cifuzz-corpus
    log_newline
  fi
fi

if [ "${DO_LINT_CPPLINT}" = "true" ] ; then
  # see https://stackoverflow.com/questions/51582604/how-to-use-cpplint-code-style-checking-with-cmake
  which cpplint >/dev/null
  if [ $? = 0 ] ; then
    log_info "Lint using cpplint"
    rm -rf ./build ; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      "-DCMAKE_CXX_CPPLINT=cpplint;--verbose=0;--quiet" -S . -B build
    (cd build ; make clean all) 2>&1 | tee lint-cpplint.log
    :
  else
    log_warn "Could not find cpplint, ignoring..."
  fi
  log_newline
fi

if [ "${DO_LINT_CLANG_TIDY}" = "true" ] ; then
  # TODO(jhi): does not run on Linux
  which clang-tidy >/dev/null
  if [ $? = 0 ] ; then
    log_info "Lint using clang-tidy"
    # add --debug-output to cmake for verbose output
    rm -rf ./build ; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      "-DCMAKE_CXX_CLANG_TIDY=clang-tidy" -S . -B build
    # add VERBOSE=1 to make for verbose output
    (cd build ; make clean all) 2>&1 | tee lint-clang-tidy.log
  else
    log_warn "Could not find clang-tidy, ignoring..."
  fi
  log_newline
fi

if [ "${DO_LINT_IWYU}" = "true" ] ; then
  # see https://include-what-you-use.org/. This linter does not really give useful results, therefoe verbose=0
  which include-what-you-use >/dev/null
  if [ $? = 0 ] ; then
    log_info "Lint using iwyu"
    # add -Xiwyu; before every iwyu flag
    rm -rf ./build ; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      "-DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=include-what-you-use;-w;-Xiwyu;--no_fwd_decls;-Xiwyu;--verbose=0" -S . -B build
    (cd build ; make clean all) 2>&1 | tee lint-iwyu.log
    :
  else
    log_warn "Could not find include-what-you-use, ignoring..."
  fi
  log_newline
fi
