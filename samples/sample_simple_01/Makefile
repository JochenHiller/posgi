CXX=clang++
# CXXFLAGS=-std=c++17 -Wall -fPIC -O2
# for development
CXXFLAGS=-std=c++17 -Wall -fPIC -g

all: hello thread core_dump string_format

hello: hello.o
	$(CXX) -o $@ $^

hello.o: hello.cc

thread: thread.o
	$(CXX) -o $@ $^

thread.o: thread.cc

core_dump: core_dump.o
	$(CXX) -o $@ $^

string_format.o: string_format.cc

string_format: string_format.o
	$(CXX) -o $@ $^

core_dump.o: core_dump.cc

include_checks: include_checks.cc
	$(CXX) $(CXXFLAGS) -c -o $@.o $^
	# suppress sytem headers: --system-headers
	clang-tidy --checks=+bugprone-suspicious-include --header-filter=.* include_checks.cc -- -std=c++17
	include-what-you-use -Xiwyu --no_fwd_decls include_checks.cc -std=c++17

.PHONY: all clean

clean:
	rm -f *.o hello thread core_dump string_format
