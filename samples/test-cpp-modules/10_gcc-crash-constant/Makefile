# compiler=clang
compiler=gcc

ifeq ($(compiler),clang)
# clang 16
CXX_HOME=/opt/homebrew/opt/llvm
CXX=${CXX_HOME}/bin/clang++
CXX_INCLUDE=${CXX_HOME}/include/c++/v1
CXX_MODULE_CACHE=.modules-cache
CXXFLAGS_MODULE_CACHE=-fmodules-cache-path=${CXX_MODULE_CACHE}
CXXFLAGS=-std=c++20 -fmodules -fprebuilt-module-path=. ${CXXFLAGS_MODULE_CACHE} -Wall -g
CXXFLAGS_MODULE=$(CXXFLAGS) -xc++-module --precompile -Wall -g
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

all: gcc

clang:
	$(CXX) -std=c++20 -xc++-system-header -fmodule-header ${CXX_INCLUDE}/iostream
	$(CXX) -std=c++20 -xc++-system-header -fmodule-header ${CXX_INCLUDE}/string
	$(CXX) $(CXXFLAGS_MODULE) m.cc -o m.pcm
	$(CXX) $(CXXFLAGS) -c m.pcm -o m.o
	$(CXX) $(CXXFLAGS) -c client.cc -o client.o
	$(CXX) ${CXX_LINK_FLAGS} m.o client.o -o main

gcc:
	$(CXX) -std=c++20 -xc++-system-header -fmodule-header ${CXX_INCLUDE}/iostream
	$(CXX) -std=c++20 -xc++-system-header -fmodule-header ${CXX_INCLUDE}/string
	$(CXX) $(CXXFLAGS_MODULE) m.cc
	$(CXX) $(CXXFLAGS) -c m.cc -o m.o
	$(CXX) $(CXXFLAGS) -c client.cc -o client.o
	$(CXX) ${CXX_LINK_FLAGS} m.o client.o -o main

.PHONY: all clean

clean:
	rm -rf gcm.cache .modules-cache
	rm -f *.o *.pcm main
