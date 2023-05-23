FRAMEWORKDIR=../../framework
CXX=clang++
CXXFLAGS=\
  -I$(FRAMEWORKDIR)/include \
  -I$(FRAMEWORKDIR)/../third_party/plog-1.1.9/include \
  -I$(FRAMEWORKDIR)/../third_party/googletest-1.13.0/googletest/include \
  -I$(FRAMEWORKDIR)/../third_party/boost_1_82_0 \
  -std=c++17 -Wall -fPIC -g

# for final build
#  -std=c++17 -Wall -fPIC -O2

all: main

main: main.o some_bundle.o \
		$(FRAMEWORKDIR)/impl/bundle_context_impl.o \
		$(FRAMEWORKDIR)/impl/bundle_impl.o \
		$(FRAMEWORKDIR)/impl/framework_impl.o \
		$(FRAMEWORKDIR)/impl/manifest_parser.o \
		$(FRAMEWORKDIR)/osgi_console.o \
		$(FRAMEWORKDIR)/framework_factory.o
	$(CXX) -v -o $@ $^

main.o: main.cc
some_bundle.o: some_bundle.cc

.PHONY: all clean

clean:
	rm -f *.o main
