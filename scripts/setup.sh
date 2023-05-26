#!/usr/bin/env bash

# TODO(JochenHiller): Use standard approach to setup project including all external dependencies
# See Python VirtualEnv with scripts/bootstrap.sh and ./scripts/activate.sh

usage() {
  cat <<EOF
Usage: $0 [clean|check|install|help]

  clean    will cleanup all third_party files
  check    will check if all required tools are installed
  install  will install all third_party files 
  help     this help
EOF
}

# parse args
DO_CLEAN="false"
DO_CHECK="false"
DO_INSTALL="false"
DO_HELP="false"
for arg in $@ ; do
  if [ "${arg}" = "clean" ] ; then
    DO_CLEAN="true"
  elif [ "${arg}" = "check" ] ; then
    DO_CHECK="true"
  elif [ "${arg}" = "install" ] ; then
    DO_INSTALL="true"
  elif [ "${arg}" = "help" ] ; then
    DO_HELP="true"
  else
    echo "WARN: Unknown command ${arg}"
    DO_HELP="true"
  fi
done

if [ "${DO_CLEAN}" = "false" ] && [ "${DO_CHECK}" = "false" ] && [ "${DO_INSTALL}" = "false" ] ; then
  DO_HELP="true"
fi

if [ "${DO_HELP}" = "true" ] ; then
  usage
  exit 1
fi


download_and_unpack_third_party() {
  do_clean=${1}     # true/false
  dir=${2}
  download_url=${3} # can be optional

  # assume tar.gz as ${dir}.tar.gz
  tar_gz=${dir}.tar.gz

  if [ "${do_clean}" = "true" ] ; then
    if [ -d "${dir}" ] ; then
      echo "WARN: Clean third_party/${dir}"
      rm -rf ${dir}
      rm -f ${tar_gz}
    fi
  elif [ ! -d "${dir}" ] ; then
    echo "INFO: Setup third_party/${dir}"
    echo "INFO: Downloading from ${download_url}"
    curl -o ${tar_gz} -L ${download_url} 2>/dev/null
    echo "INFO: Unpacking third_party/${tar_gz}"
    tar -xzf ${tar_gz}
  else
    echo "INFO: third_party/${dir} already setup"
  fi
}

check_one_tool() {
  binary=${1}
  required=${2}    # can be required or optional
  column=${3}      # can be optional
  min_version=${4} # can be optional

   # check for one tool
  which ${binary} >/dev/null
  if [ ! $? = 0 ] ; then
    if [ "${required}" = "required" ] ; then
      echo "WARN: ${binary} not found (is required)"
    else
      echo "INFO: ${binary} not found (is optional)"
    fi
  else
    if [ "${column}" = "" ] || [ "${min_version}" = "" ]; then
      # ignore version check
      echo "INFO: ${binary} found"
    else
      _VERSION=$(${binary} --version | grep "version" | cut -f ${column} -d " ")
      if [ "${_VERSION}" = "" ] ; then
        # hack for ubuntu g++
        _VERSION=$(${binary} --version | grep "ubuntu" | cut -f ${column} -d " ")
      fi
      if [ "${_VERSION}" = "" ] ; then
         # hack for cpplint
        _VERSION=$(${binary} --version | grep -v "fork" | grep "cpplint" | cut -f ${column} -d " ")
      fi
      if [ "${_VERSION}" = "" ] ; then
        echo "WARN: ${binary} version not found"
      else
        if [[ "${_VERSION}" < ${min_version} ]] ; then
          echo "WARN: ${binary} too old (${min_version} required): found ${_VERSION} "
        else
          echo "INFO: ${binary} version ${_VERSION} found"
        fi
      fi
    fi
  fi
}

check_all_tools() {
  # check for clang compiler >= 16.x
  # clang++ --version ==> Homebrew clang version 16.0.4
  check_one_tool clang++ required 4 15

  # check for clang format >= 16.x
  # clang-format --version ==> Homebrew clang-format version 16.0.4
  check_one_tool clang-format required 4 15

  # check for clang tidy >= 16.x
  # clang-tidy --version ==> Homebrew LLVM version 16.0.4
  check_one_tool clang-tidy required 4 15

  # check for lldb debugger >= 16.x
  # lldb --version ==> lldb version 16.0.4
  check_one_tool lldb required 3 15

  # check for g++ compiler >= 16.x
  # g++ --version ==> Apple clang version 14.0.3 (clang-1403.0.22.14.1)
  check_one_tool g++ optional 4 12

  # check for cmake
  # cmake --version ==> cmake version 3.26.4
  check_one_tool cmake required 3 3

  # check for cmake-format
  # see https://github.com/cheshirekow/cmake_format
  # install with: "pip install cmakelang"
  # cmake-format --version ==> 0.6.13
  check_one_tool cmake-format optional

  # check for cpplint
  # cpplint --version cpplint 1.6.1
  check_one_tool cpplint optional 2 1.6

  # check for include-what-you-use >= 16.x
  # include-what-you-use --version ==> include-what-you-use 0.20 based on Homebrew clang version 16.0.4
  check_one_tool include-what-you-use optional 2 0.20
}

# see https://github.com/zemasoft/clangformat-cmake
cmake_clang_format() {
  do_clean=${1} # true/false
  download_url=https://raw.githubusercontent.com/zemasoft/clangformat-cmake/master/cmake/ClangFormat.cmake
  dir=cmake

  cd ../third_party
  if [ "${do_clean}" = "true" ] ; then
    echo "WARN: Clean third_party/clangformat-cmake"
    rm -rf ${dir}
  elif [ ! -d "${dir}" ] ; then
    echo "INFO: Setup clangformat-cmake"
    echo "INFO: Downloading from ${download_url}"
    echo "INFO: Installing clangformat-cmake"
    wget ${download_url} -P ${dir} 2>/dev/null

    echo "INFO: Patching clangformat-cmake for verbose output"
    sed -e 's|-style=file|-style=file --verbose|g' cmake/ClangFormat.cmake >cmake/ClangFormat.cmake.tmp
    mv cmake/ClangFormat.cmake.tmp cmake/ClangFormat.cmake
  else
    echo "INFO: clangformat-cmake already setup"
  fi
}

build_googletest() {
  dir=${1}
  echo "INFO: Build googletest in third_party/${dir}"

  (
  cd ${dir}
  cmake -S . -B build >/dev/null
  (cd build ; make >/dev/null)
  # libs are in ./third_party/googletest-1.13.0/build/lib
  )
}

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd ${SCRIPT_DIR}

if [ "${DO_CLEAN}" = "true" ] && [ -d ../third_party ] ; then
  (
    cd ../third_party

    echo "INFO: Clean third_party ..."
    download_and_unpack_third_party true plog-1.1.9
    download_and_unpack_third_party true googletest-1.13.0
    download_and_unpack_third_party true boost_1_82_0

    cmake_clang_format true
  )

  if [ -d ../third_party ] ; then
    echo "WARN: Removing third_party"
    rmdir ../third_party
    echo " "
  fi
fi

if [ "${DO_CHECK}" = "true" ] ; then
  echo "INFO: Checking installed tools ..."
  check_all_tools
  echo " "
fi

if [ "${DO_INSTALL}" = "true" ] ; then
  echo "INFO: Checking third_party libraries ..."

  cd ${SCRIPT_DIR}
  if [ ! -d ../third_party ] ; then
    echo "INFO: Creating third_party/"
    mkdir -p ../third_party
  fi

  if [ -d ../third_party ] ; then
  (
    cd ../third_party

    # plog: https://github.com/SergiusTheBest/plog
    # plog is a portable and simple logging library for C++ programs.
    download_and_unpack_third_party false plog-1.1.9 \
      https://github.com/SergiusTheBest/plog/archive/refs/tags/1.1.9.tar.gz

    # gtest: https://github.com/google/googletest
    # gtest is a standard framework to write unit tests on C++
    download_and_unpack_third_party false googletest-1.13.0 \
      https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz
    build_googletest googletest-1.13.0

    # boost: https://www.boost.org/doc/libs/1_82_0/more/getting_started/unix-variants.html
    # boost is a standard library for C++
    download_and_unpack_third_party false boost_1_82_0 \
      https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz
    )

    cmake_clang_format false
  fi
fi
