CXX=clang++
CXXFLAGS=\
  -I./include \
  -I../third_party/plog-1.1.9/include \
  -I../third_party/googletest-1.13.0/googletest/include \
  -I../third_party/boost_1_82_0 \
  -std=c++17 -Wall -fPIC -g

# for final build
#  -std=c++17 -Wall -fPIC -O2

all: posgi

posgi: \
		impl/bundle_context_impl.o \
		impl/bundle_impl.o \
		impl/framework_impl.o \
		impl/manifest_parser.o \
		osgi_console.o \
		framework_factory.o \
		main.o
	$(CXX) -o $@ $^

impl/bundle_context_impl.o: impl/bundle_context_impl.cc
impl/bundle_impl.o: impl/bundle_impl.cc
impl/framework_impl.o: impl/framework_impl.cc
impl/manifest_parser.o: impl/manifest_parser.cc
osgi_console.o: osgi_console.cc
framework_factory.o: framework_factory.cc
main.o: main.cc

.PHONY: all clean

clean:
	rm -f *.o impl/*.o posgi
