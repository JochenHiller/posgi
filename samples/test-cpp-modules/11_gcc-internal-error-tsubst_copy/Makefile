# compiler=clang
# compiler=gcc

ifeq ($(compiler),clang)
# clang 16
CXX_HOME=/opt/homebrew/opt/llvm
CXX=${CXX_HOME}/bin/clang++
CXXFLAGS=-std=c++20 -fmodules -Wall -g
endif
ifeq ($(compiler),gcc)
# gcc 13
CXX_HOME=/opt/homebrew/Cellar/gcc/13.1.0
CXX=${CXX_HOME}/bin/gcc-13
CXX_INCLUDE=${CXX_HOME}/include/c++/13
CXXFLAGS=-std=c++20 -fmodules-ts -Wall -g -MMD
CXXFLAGS_MODULE=$(CXXFLAGS) -fmodule-only
endif
CXX_INCLUDE_PATH=\
  -I../../../third_party/plog-1.1.9/include \

all: gcc

clang:
	$(CXX) $(CXXFLAGS) -xc++-module --precompile i.cc -o i.pcm
	$(CXX) $(CXXFLAGS) ${CXX_INCLUDE_PATH} -xc++-module --precompile -fmodule-file=i.pcm m_impl.cc -o m_impl.pcm
	$(CXX) $(CXXFLAGS) -xc++-module --precompile -fmodule-file=i.pcm -fmodule-file=m_impl.pcm m_public.cc -o m_public.pcm
	$(CXX) $(CXXFLAGS) -xc++-module --precompile -fmodule-file=m_public.pcm m.cc -o m.pcm
	$(CXX) $(CXXFLAGS) -c i.pcm -o i.o
	$(CXX) $(CXXFLAGS) -c m_impl.pcm -o m_impl.o
	$(CXX) $(CXXFLAGS) -c m_public.pcm -o m_public.o
	$(CXX) $(CXXFLAGS) -c m.pcm -o m.o

gcc:
	$(CXX) -std=c++20 -xc++-system-header -fmodule-header ${CXX_INCLUDE}/string
	$(CXX) $(CXXFLAGS) ${CXX_INCLUDE_PATH} -c i.cc -o i.o
	$(CXX) $(CXXFLAGS) ${CXX_INCLUDE_PATH} -c m_impl.cc -o m_impl.o
	$(CXX) $(CXXFLAGS) ${CXX_INCLUDE_PATH} -c m_public.cc -o m_public.o
	$(CXX) $(CXXFLAGS) ${CXX_INCLUDE_PATH} -freport-bug -c m.cc -o m.o

.PHONY: all clean

clean:
	rm -rf gcm.cache .modules-cache
	rm -f *.log *.d *.o *.pcm *.pcm.info main
