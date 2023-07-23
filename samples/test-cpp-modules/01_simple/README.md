# Test C++ Modules: Simple tests

Based on article from Rainer Grimm here:

* <https://www.modernescpp.com/index.php/cpp20-a-first-module>
* <https://www.modernescpp.com/index.php/tag/modules>

Notes:

* `import <system-library>;` after `export module`, not in global module scope
* Use system libraries with same name as old header files, use in brackets `import <system-library-name>;`
  * e.g. `import <iostream>;`
* Change compiler between clang and gcc in Makefile
* Clang compiler:
  * no need to precompile standard libraries, clang knows them by default
  * if wished: we can use local cache by using `-fmodules-cache-path=pcm.cache`
* Gcc compiler:
  * we need to precompile standard headers to be able to import them
  * precompiled modules goes by default into `gcm.cache` folder

```bash
# clang: precompile system headers via script, not needed
clang++ -std=c++20 -xc++-system-header -fmodules-cache-path=pcm.cache --precompile iostream -o iostream.pcm
clang++ -std=c++20 -xc++-system-header -fmodules-cache-path=pcm.cache --precompile vector -o vector.pcm
clang++ -std=c++20 -xc++-system-header -fmodules-cache-path=pcm.cache --precompile numeric -o numeric.pcm

# gcc: precompile system headers via script, needed, goes into gcm.cache
export GCC_HOME=/opt/homebrew/Cellar/gcc/13.1.0
export PATH=${GCC_HOME}:${PATH}
gcc-13 --version
rm -rf gcm.cache
gcc-13 -std=c++20 -xc++-system-header -fmodule-header ${GCC_HOME}/include/c++/13/iostream
gcc-13 -std=c++20 -xc++-system-header -fmodule-header ${GCC_HOME}/include/c++/13/vector
gcc-13 -std=c++20 -xc++-system-header -fmodule-header ${GCC_HOME}/include/c++/13/numeric
find gcm.cache

# test
make clean all ; ./main ; make clean
```
