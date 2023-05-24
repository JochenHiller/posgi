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

check_tools() {
  # check for clang compiler >= 16.x
  which clang++ >/dev/null
  if [ ! $? = 0 ] ; then
    echo "WARN: clang++ compiler not found"
  else
    CLANG_PLUSPLUS_VERSION=$(clang++ --version | grep version | awk '{print $4}')
    if [[ "${CLANG_PLUSPLUS_VERSION}" < "16" ]] ; then
      echo "WARN: clang++ compiler too old (16+ required): found ${CLANG_PLUSPLUS_VERSION} "
    else
      echo "INFO: clang++ compiler version ${CLANG_PLUSPLUS_VERSION} found"
    fi
  fi

  # check for clang format >= 16.x
  which clang-format >/dev/null
  if [ ! $? = 0 ] ; then
    echo "WARN: clang-format not found"
  else
    CLANG_FORMAT_VERSION=$(clang-format --version | grep version | awk '{print $4}')
    if [[ "${CLANG_FORMAT_VERSION}" < "16" ]] ; then
      echo "WARN: clang-format too old (16+ required): found ${CLANG_FORMAT_VERSION}"
    else
      echo "INFO: clang-format version ${CLANG_FORMAT_VERSION} found"
    fi
  fi

  # check for lldb debugger >= 16.x
  which lldb >/dev/null
  if [ ! $? = 0 ] ; then
    echo "WARN: lldb not found"
  else
    LLDB_VERSION=$(lldb --version | grep version | awk '{print $3}')
    if [[ "${LLDB_VERSION}" < "16" ]] ; then
      echo "WARN: lldb too old (16+ required): found ${LLDB_VERSION}"
    else
      echo "INFO: lldb version ${LLDB_VERSION} found"
    fi
  fi

  # check for g++ compiler >= 16.x
  which g++ >/dev/null
  if [ ! $? = 0 ] ; then
    echo "WARN: g++ compiler not found"
  else
    GPLUSPLUS_VERSION=$(g++ --version | grep version | awk '{print $4}')
    if [[ "${GPLUSPLUS_VERSION}" < "14" ]] ; then
      echo "WARN: g++ compiler too old (14+ required): found ${GPLUSPLUS_VERSION} "
    else
      echo "INFO: g++ compiler version ${GPLUSPLUS_VERSION} found"
    fi
  fi

  # check for cmake
  which cmake >/dev/null
  if [ ! $? = 0 ] ; then
    echo "WARN: cmake not found"
  else
    CMAKE_VERSION=$(cmake --version | grep version | awk '{print $3}')
    if [[ "${CMAKE_VERSION}" < "3" ]] ; then
      echo "WARN: cmake too old (3+ required): found ${CMAKE_VERSION} "
    else
      echo "INFO: cmake version ${CMAKE_VERSION} found"
    fi
  fi

  # check for cmake-format
  # see https://github.com/cheshirekow/cmake_format
  # install with: "pip install cmakelang"
  which cmake-format >/dev/null
  if [ ! $? = 0 ] ; then
    echo "WARN: cmake-format not found"
  else
    CMAKE_FORMAT_VERSION=$(cmake-format --version)
    if [[ "${CMAKE_FORMAT_VERSION}" < "0.6" ]] ; then
      echo "WARN: cmake-format too old (3+ required): found ${CMAKE_FORMAT_VERSION} "
    else
      echo "INFO: cmake-format version ${CMAKE_FORMAT_VERSION} found"
    fi
  fi
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
