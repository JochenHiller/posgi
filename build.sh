#!/usr/bin/env bash

if [ "${1}" = "clean" ] ; then
  rm -vf *.o framework/*.o framework/impl/*.o samples/sample_simple_0?/*.o
  rm -vf framework/posgi samples/sample_simple_0?/main
  shift
fi

if [ "${1}" = "fmt" ] ; then
  rm -f files_to_fmt.txt
  find framework -name "*.h" >>files_to_fmt.txt
  find framework -name "*.cc" >>files_to_fmt.txt
  find samples -name "*.h" >>files_to_fmt.txt
  find samples -name "*.cc" >>files_to_fmt.txt
  # for a dry-run
  # clang-format -style=Google --dry-run -files=files_to_fmt.txt
  # for real formatting
  clang-format -style=Google --verbose -i -files=files_to_fmt.txt
  rm -f files_to_fmt.txt
  shift
fi

if [ "${1}" = "compile" ] || [ -n "${1}" ] ; then

  cd framework ; make clean all ; ls -al *.o */*.o posgi ; cd ..

  cd samples/sample_simple_02 ; make clean all ; ls -al *.o main ; cd ../..

  cd samples/sample_simple_03 ; make clean all ; ls -al *.o main ; cd ../..
  ./samples/sample_simple_03/main
  shift
fi
