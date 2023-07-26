# compiler=clang
# compiler=gcc

ifeq ($(compiler),"")
  compiler=clang
endif

ifeq ($(compiler),clang)
  # clang 16
  CXX=clang++
  CXX_LINK_FLAGS=-stdlib=libc++
endif
ifeq ($(compiler),gcc)
  # gcc 13
  CXX_HOME=/opt/homebrew/Cellar/gcc/13.1.0
  CXX=${CXX_HOME}/bin/gcc-13
  CXX_LINK_FLAGS=-lstdc++
endif

# release version
# CXXFLAGS=\
  -I../../third_party/plog-1.1.9/include \
  -I../../third_party/boost_1_82_0 \
  -std=c++17 -Wall -fPIC -O2 -g

# for development
CXXFLAGS=\
  -I../../third_party/plog-1.1.9/include \
  -I../../third_party/boost_1_82_0 \
  -std=c++17 -Wall -fPIC -g

all: main

main: main.o
	$(CXX) ${CXX_LINK_FLAGS} -o $@ $^

main.o: main.cc

.PHONY: all clean

clean:
	rm -f *.log *.o main
