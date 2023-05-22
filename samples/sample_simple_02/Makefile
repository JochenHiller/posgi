CXX=clang++
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
	$(CXX) -v -o $@ $^

main.o: main.cc

.PHONY: all clean

clean:
	rm -f *.o main
