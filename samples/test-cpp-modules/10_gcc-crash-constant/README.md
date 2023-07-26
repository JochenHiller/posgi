# Submit GCC Bug

## Overview

* GCC Bugzilla: <https://gcc.gnu.org/bugzilla/>
  * Account needed and requested
* Check known bugs: <https://gcc.gnu.org/bugs/#known>
* Submit a bug: see <https://gcc.gnu.org/bugs/>

* Module related bugs:
  * <https://gcc.gnu.org/bugzilla/show_bug.cgi?id=104523>
  * <https://gcc.gnu.org/bugzilla/showdependencytree.cgi?id=103524&hide_resolved=1>

Testing:

```bash
export PATH=/opt/homebrew/Cellar/gcc/13.1.0/bin:$PATH
gcc-13 --version
```

## Bug submission

* Bug reported: <https://gcc.gnu.org/bugzilla/show_bug.cgi?id=110805>

```plain
Title: g++ crash on modules with exported class providing string constant with obscure content
Product: gcc
Comp: c++

// Info
gcc-13 --version
gcc-13 (Homebrew GCC 13.1.0) 13.1.0
macOS 13.5, arm64, Apple M1 MAX

// Steps to reproduce
* use a C++20 module with exported class providing obscure string constant
* gcc 13.1 crash with "m.cc:3:8: internal compiler error: Segmentation fault: 11"

// m.cc
module;
#include <string>
export module m;
export class Constants {
 public:
  inline static const std::string CONST_CRASH_1 = "Bundle-Activator";
};

// build
gcc-13 -freport-bug -std=c++20 -xc++ -fmodules-ts -fmodule-only -Wall -Wextra m.cc
# see attached output file ccRJ91Vu.out.txt

// notes
* exported string constants outside exported class do work
* string constants with different types/values do work as well, see different string value, very obscure
  static constexpr std::string_view CONST_OK_1 = "Bundle-Activator";
  inline static const std::string CONST_OK_2 = "bundleactivator";
  inline static const std::string CONST_OK_3 = "Bundle-Activato";
  inline static const std::string CONST_OK_4 = "undle-Activator";
  inline static const std::string CONST_OK_5 = "xxxXactivatorll";
* these string constants do fail as well
  inline static const std::string CONST_CRASH_2 = "xxxbundleXactivatorlll";
  inline static const std::string CONST_CRASH_3 = "xxxXactivatorlll";
* it seems this pattern ".*.activator.*" triggeres special handling in gcc-13, lower/upper case seems not to be relevant, but number of characters before/after
```

## Open issues

* Why will Constants.CONST_OK_1 not be resolved in client.cc using gcc-13? It does work in Clang.
  * Another bug to report? First check if that code is valid C++20 code.

```bash
objdump -D m.o | grep -i CONST
objdump -D client.o | grep -i CONST
strings gcm.cache/m.gcm | grep -i CONST
```
