CXX=clang++
# CXXFLAGS=-std=c++17 -Wall -fPIC -O2
# for development
CXXFLAGS=-std=c++17 -Wall -fPIC -g

all: hello thread coredump

hello: hello.o
	$(CXX) -o $@ $^

hello.o: hello.cc

thread: thread.o
	$(CXX) -o $@ $^

thread.o: thread.cc

coredump: coredump.o
	$(CXX) -o $@ $^

coredump.o: coredump.cc

.PHONY: all clean

clean:
	rm -f *.o hello thread coredump
