#!/usr/bin/env bash

usage() {
  cat <<EOF
Usage: $0 [clean|install|help]

  install  will install all required libraries and check for installed tools 
  clean    will cleanup all installed files
  help     this help
EOF
}
# parse args
DO_CLEAN="false"
DO_INSTALL="false"
DO_HELP="false"
for arg in $@ ; do
  if [ "${arg}" = "clean" ] ; then
    DO_CLEAN="true"
  elif [ "${arg}" = "install" ] ; then
    DO_INSTALL="true"
  elif [ "${arg}" = "help" ] ; then
    DO_HELP="true"
  else
    echo "WARN: Unknown command ${arg}"
    DO_HELP="true"
  fi
done

if [ "${DO_CLEAN}" = "false" ] && [ "${DO_INSTALL}" = "false" ] ; then
  DO_HELP="true"
fi

if [ "${DO_HELP}" = "true" ] ; then
  usage
  exit 1
fi


download_and_unpack_third_party() {
  dir=${1}
  download_url=${2}
  do_clean=${3}

  # assume tar.gz as ${dir}.tar.gz
  tar_gz=${dir}.tar.gz

  if [ "${do_clean}" = "true" ] ; then
    if [ -d "${dir}" ] ; then
      echo "WARN: Clean ${dir} ..."
      rm -rf ${dir}
      rm -f ${tar_gz}
    fi
  elif [ ! -d "${dir}" ] ; then
    echo "INFO: Setup ${dir} ..."
    echo "INFO: Downloading from ${download_url} ..."
    curl -o ${tar_gz} -L ${download_url} 2>/dev/null
    echo "INFO: Unpacking ${tar_gz} ..."
    tar -xzf ${tar_gz}
  else
    echo "INFO: ${dir} already setup"
  fi
}


check_one_tool() {
  binary=${1}
  column=${2}
  min_version=${3}
   # check for one tool
  which ${binary} >/dev/null
  if [ ! $? = 0 ] ; then
    echo "WARN: ${binary} not found"
  else
    if [ "${column}" = "" ] || [ "${min_version}" = "" ]; then
      # ignore version check
      echo "INFO: ${binary} found"
    else
      _VERSION=$(${binary} --version | grep version | cut -f ${column} -d " ")
      if [[ "${_VERSION}" < ${min_version} ]] ; then
        echo "WARN: ${binary} too old (${min_version} required): found ${_VERSION} "
      else
        echo "INFO: ${binary} version ${_VERSION} found"
      fi
    fi
  fi
}

check_tools() {
  # check for clang compiler >= 16.x
  # clang++ --version ==> Homebrew clang version 16.0.4
  check_one_tool clang++ 4 16

  # check for clang format >= 16.x
  # clang-format --version ==> Homebrew clang-format version 16.0.4
  check_one_tool clang-format 4 16

  # check for clang tidy >= 16.x
  # clang-tidy --version ==> Homebrew LLVM version 16.0.4
  check_one_tool clang-tidy 4 16

  # check for lldb debugger >= 16.x
  # lldb --version ==> lldb version 16.0.4
  check_one_tool lldb 3 16

  # check for g++ compiler >= 16.x
  # g++ --version ==> Apple clang version 14.0.3 (clang-1403.0.22.14.1)
  check_one_tool g++ 4 14

  # check for cmake
  # cmake --version ==> cmake version 3.26.4
  check_one_tool cmake 3 3

  # check for cmake-format
  # see https://github.com/cheshirekow/cmake_format
  # install with: "pip install cmakelang"
  # cmake-format --version ==> 0.6.13
  check_one_tool cmake-format

  # check for cpplint
  # include-what-you-use --version ==> include-what-you-use 0.20 based on Homebrew clang version 16.0.4
  check_one_tool cpplint

  # check for include-what-you-use >= 16.x
  # include-what-you-use --version ==> include-what-you-use 0.20 based on Homebrew clang version 16.0.4
  check_one_tool include-what-you-use 2 0.20
}

# see https://github.com/zemasoft/clangformat-cmake
cmake_clang_format() {
  do_clean=${1}
  download_url=https://raw.githubusercontent.com/zemasoft/clangformat-cmake/master/cmake/ClangFormat.cmake
  dir=cmake

  cd ../third_party
  if [ "${do_clean}" = "true" ] ; then
    echo "WARN: Clean clangformat-cmake ..."
    rm -rf ${dir}
  elif [ ! -d "${dir}" ] ; then
    echo "INFO: Setup clangformat-cmake ..."
    echo "INFO: Downloading from ${download_url} ..."
    echo "INFO: Installing clangformat-cmake ..."
    wget ${download_url} -P ${dir} 2>/dev/null

    echo "INFO: Patching clangformat-cmake for verbose output ..."
    sed -e 's|-style=file|-style=file --verbose|g' cmake/ClangFormat.cmake >cmake/ClangFormat.cmake.tmp
    mv cmake/ClangFormat.cmake.tmp cmake/ClangFormat.cmake
  else
    echo "INFO: clangformat-cmake already setup"
  fi
}

# TODO Use standard approach to setup project including all external dependencies
# See Python VirtualEnv with scripts/bootstrap.sh and ./scripts/activate.sh

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd ${SCRIPT_DIR}

if [ "${DO_CLEAN}" = "true" ] && [ -d ../third_party ] ; then
  (
    cd ../third_party

    # plog: https://github.com/SergiusTheBest/plog
    # plog is a portable and simple logging library for C++ programs.
    download_and_unpack_third_party \
      plog-1.1.9 https://github.com/SergiusTheBest/plog/archive/refs/tags/1.1.9.tar.gz \
      ${DO_CLEAN}

    # gtest: https://github.com/google/googletest
    # gtest is a standard framework to write unit tests on C++
    download_and_unpack_third_party \
      googletest-1.13.0 https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz \
      ${DO_CLEAN}

    # boost: https://www.boost.org/doc/libs/1_82_0/more/getting_started/unix-variants.html
    # boost is a standard library for C++
    download_and_unpack_third_party \
      boost_1_82_0 https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz \
      ${DO_CLEAN}

    cmake_clang_format ${DO_CLEAN}
  )

  if [ -d ../third_party ] ; then
    echo "WARN: Removing third_party directory ..."
    rmdir ../third_party
    echo " "
  fi
fi


if [ "${DO_INSTALL}" = "true" ] ; then
  echo "INFO: Checking installed tools ..."
  check_tools
  echo " "

  echo "INFO: Checking C++ libraries ..."

  cd ${SCRIPT_DIR}
  if [ ! -d ../third_party ] ; then
    echo "INFO: Creating third_party directory ..."
    mkdir -p ../third_party
  fi

  if [ -d ../third_party ] ; then
  (
    cd ../third_party

    # plog: https://github.com/SergiusTheBest/plog
    # plog is a portable and simple logging library for C++ programs.
    download_and_unpack_third_party \
      plog-1.1.9 https://github.com/SergiusTheBest/plog/archive/refs/tags/1.1.9.tar.gz

    # gtest: https://github.com/google/googletest
    # gtest is a standard framework to write unit tests on C++
    download_and_unpack_third_party \
      googletest-1.13.0 https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz

    # boost: https://www.boost.org/doc/libs/1_82_0/more/getting_started/unix-variants.html
    # boost is a standard library for C++
    download_and_unpack_third_party \
      boost_1_82_0 https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz
    )

    cmake_clang_format
  fi
fi
