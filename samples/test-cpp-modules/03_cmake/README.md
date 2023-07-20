# Test C++20 Modules: Tests with interface / implementation separation using CMake

Based on article from Rainer Grimm here:

* <https://www.modernescpp.com/index.php/c-20-module-interface-unit-and-module-implementation-unit>
* <https://www.modernescpp.com/index.php/tag/modules>
* Using [CMake extension](https://github.com/vitaut/modules) from [Victor Zverovich](https://github.com/vitaut)
  * Not the ["offical" C++ Modules](https://www.kitware.com/import-cmake-c20-modules/) support modules, but requiring Ninja

Notes:

* We did just test it with latest Clang 16 and GCC 13
* the [modules.cmake](./modules.cmake) needed to be patched (file [modules-patched.cmake](./modules-patched.cmake))
  * add `-fmodules` when compiling C++ code (providing modules, using modules)
  * for convenience, we set the cache path to a local directory, e.g. `-fmodules-cache-path=.modules-cache`
* We need to add `-fmodule-file=${CMAKE_CURRENT_BINARY_DIR}/math.pcm` to compile option for a file, which has not a transitive dependency found by CMake (e.g. here the implementation library, as we just use add_library)
* Clang:
  * we did set `-fmodules` globally as compile option for ALL C++ files, to avoid to add it to all non-transitive dependencies which are not related to a module library
* Gcc:
  * we need to set `-fmodules-ts` globally as compile option, as modules.cmake does not yet support GCC
  * we need to add `-lstdc++` to link options
  * we did not find an easy way to first precompile system headers. We are doing that BEFORE the build/make will be started. We do use the defult `gcm.cache` in build dir
* on macOS 13.4, there was a conflict between different installation directories
  * The fix was to configure in Cmake sysroot to the expected one installation (`/Library/...`)

```bash
# Compilation errors
[ 66%] Building CXX object CMakeFiles/main.dir/client.cc.o
/opt/homebrew/opt/llvm/bin/clang++   -std=c++20 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.3.sdk -fmodules -fmodule-file=/System/Volumes/Data/data/p/posgi/git/posgi/samples/test-modules-03-cmake/build/math.pcm -MD -MT CMakeFiles/main.dir/client.cc.o -MF CMakeFiles/main.dir/client.cc.o.d -o CMakeFiles/main.dir/client.cc.o -c /System/Volumes/Data/data/p/posgi/git/posgi/samples/test-modules-03-cmake/client.cc
fatal error: module 'Darwin' was built in directory '/Library/Developer/CommandLineTools/SDKs/MacOSX13.sdk/usr/include' but now resides in directory '/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.3.sdk/usr/include'
note: imported by module 'std' in '/var/folders/_f/yfc6xzvd2sd1gsvthzlfkbdm0000gn/C/clang/ModuleCache/3TVI46CVMGGC2/std-KVFF9BVQTOLV.pcm'
note: imported by module 'math' in '/System/Volumes/Data/data/p/posgi/git/posgi/samples/test-modules-03-cmake/build/math.pcm'
1 error generated.
```

```cmake
# Fix in CMakeLists.txt
set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX13.sdk")
```

Snippets to test build:

```bash
# Testing with clang
rm -rf build ; CXX=clang++ cmake -S . -B build
(cd build ; make clean all ; ./main)
rm -rf build

# Testing with clang and verbose output
rm -rf build ; CXX=clang++ cmake --debug-output -S . -B build
(cd build ; rm -rf pcm.cache ; VERBOSE=1 make clean all ; ./main)

# Testing with gcc and verbose output
export GCC_HOME=/opt/homebrew/Cellar/gcc/13.1.0
export PATH=${GCC_HOME}/bin:${PATH}
export CXX=${GCC_HOME}/bin/gcc-13
${CXX} --version

rm -rf build ; CXX=gcc-13 cmake --debug-output -S . -B build
(cd build ; ../gcc13_gen_system_modules.sh ; VERBOSE=1 make clean all ; ./main)
rm -rf build
```
