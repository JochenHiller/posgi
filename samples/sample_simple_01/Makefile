CXX=g++
CXXFLAGS=-std=c++17 -Wall -fPIC -O2

all: main

main: main.o
	$(CXX) -o $@ $^

main.o: main.cc

.PHONY: all clean

clean:
	rm -f *.o main
