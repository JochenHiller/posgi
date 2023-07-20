# compiler=clang
compiler=gcc

ifeq ($(compiler),clang)
  # clang 16
  CXX_HOME=/opt/homebrew/opt/llvm
  CXX=${CXX_HOME}/bin/clang++
  CXX_INCLUDE=${CXX_HOME}/include/c++/v1
  CXXFLAGS=\
    -std=c++20 -fmodules -fprebuilt-module-path=. \
    -Wall -g
  CXXFLAGS_MODULE=\
    $(CXXFLAGS) -Xclang -emit-module-interface --precompile
  CXX_LINK_FLAGS=-stdlib=libc++ 
endif
ifeq ($(compiler),gcc)
  # gcc 13
  CXX_HOME=/opt/homebrew/Cellar/gcc/13.1.0
  CXX=${CXX_HOME}/bin/gcc-13
  CXX_INCLUDE=${CXX_HOME}/include/c++/13
  CXXFLAGS=-std=c++20 -fmodules-ts -Wall -g
  CXXFLAGS_MODULE=$(CXXFLAGS) -fmodule-only
  CXX_LINK_FLAGS=-lstdc++
endif


all: std_libraries main

main: math.o client.o
	$(CXX) ${CXX_LINK_FLAGS} -o $@ $^

client.o: client.cc math.cc

math.o: math.cc math.cc

math.pcm: math.cc
	$(CXX) $(CXXFLAGS_MODULE) math.cc -o math.pcm

std_libraries:
	$(CXX) -std=c++20 -xc++-system-header -fmodule-header ${CXX_INCLUDE}/iostream
	$(CXX) -std=c++20 -xc++-system-header -fmodule-header ${CXX_INCLUDE}/vector
	$(CXX) -std=c++20 -xc++-system-header -fmodule-header ${CXX_INCLUDE}/numeric

.PHONY: all clean

clean:
	rm -rf gcm.cache
	rm -f *.o *.pcm main
