# compiler=clang
compiler=gcc

ifeq ($(compiler),clang)
  # clang 16
  CXX_HOME=/opt/homebrew/opt/llvm
  CXX=${CXX_HOME}/bin/clang++
  CXX_INCLUDE=${CXX_HOME}/include/c++/v1
  CXXFLAGS_MODULE_CACHE=-fmodules-cache-path=.modules-cache
  CXXFLAGS=\
    -std=c++20 -fmodules -fprebuilt-module-path=. ${CXXFLAGS_MODULE_CACHE}  \
    -Wall -g
  CXXFLAGS_MODULE=\
    $(CXXFLAGS) -Xclang -emit-module-interface --precompile \
    -Wall -g
  CXX_LINK_FLAGS=-stdlib=libc++ 
endif
ifeq ($(compiler),gcc)
  # gcc 13
  CXX_HOME=/opt/homebrew/Cellar/gcc/13.1.0
  CXX=${CXX_HOME}/bin/gcc-13
  CXX_INCLUDE=${CXX_HOME}/include/c++/13
  CXXFLAGS_MODULE_CACHE=
  CXXFLAGS=-std=c++20 -fmodules-ts -fmodules-cache-path=.modules-cache -Wall -g
  CXXFLAGS_MODULE=$(CXXFLAGS) -fmodule-only
  CXX_LINK_FLAGS=-lstdc++
endif


all: std_libraries main

main: math_impl.o math.o client.o
	$(CXX) ${CXX_LINK_FLAGS} -o $@ $^

client.o: client.cc math.pcm

math_impl.o: math_impl.cc math.pcm

math.o: math.cc math.pcm

math.pcm: math.cc
	$(CXX) $(CXXFLAGS_MODULE) math.cc -o math.pcm

std_libraries:
	$(CXX) -std=c++20 -xc++-system-header ${CXXFLAGS_MODULE_CACHE} -fmodule-header ${CXX_INCLUDE}/iostream
	$(CXX) -std=c++20 -xc++-system-header ${CXXFLAGS_MODULE_CACHE} -fmodule-header ${CXX_INCLUDE}/vector
	$(CXX) -std=c++20 -xc++-system-header ${CXXFLAGS_MODULE_CACHE} -fmodule-header ${CXX_INCLUDE}/numeric

.PHONY: all clean

clean:
	rm -rf gcm.cache .modules-cache
	rm -f *.o *.pcm main
