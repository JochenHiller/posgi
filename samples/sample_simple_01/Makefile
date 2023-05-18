CXX=clang++
CXXFLAGS=-std=c++17 -Wall -fPIC -O2

all: hello thread

hello: hello.o
	$(CXX) -o $@ $^

hello.o: hello.cc

thread: thread.o
	$(CXX) -o $@ $^

thread.o: thread.cc

.PHONY: all clean

clean:
	rm -f *.o hello thread
