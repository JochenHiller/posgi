# References

These references have been found during development, which are worth to be kept.

## OSGi specifications

* <https://docs.osgi.org/specification/>
* OSGi R8 - Specification
  * [OSGi Core Release 8 Specification](https://docs.osgi.org/specification/osgi.core/8.0.0/) ([pdf](https://docs.osgi.org/download/r8/osgi.core-8.0.0.pdf))
  * [OSGi Compendium Release 8 Specification](https://docs.osgi.org/specification/osgi.cmpn/8.0.0/) ([pdf](https://docs.osgi.org/download/r8/osgi.cmpn-8.0.0.pdf))
  * [OSGi Compendium Release 8.1 Specification](https://docs.osgi.org/specification/osgi.cmpn/8.1.0/) ([pdf](https://docs.osgi.org/download/r8/osgi.cmpn-8.1.0.pdf))
  * [RFC 132 (Commandline Proposal from 4.2-early-draft)](osgi-r8/osgi-RFC-132-Draft.pdf)

* OSGi R8 - Documentation
  * [OSGi Annotation Release 8 Javadoc](https://docs.osgi.org/javadoc/osgi.annotation/8.1.0/)
  * [OSGi Core Release 8 Javadoc](https://docs.osgi.org/javadoc/osgi.core/8.0.0/)
  * [OSGi Compendium Release 8 Javadoc](https://docs.osgi.org/javadoc/osgi.cmpn/8.0.0/)
  * [OSGi Compendium Release 8.1 Javadoc](https://docs.osgi.org/javadoc/osgi.cmpn/8.1.0/)
* OSGi R8 - Artifacts
  * [OSGi Core](https://central.sonatype.com/artifact/org.osgi/osgi.core/8.0.0)
  * [OSGi Annotations 8.1](https://central.sonatype.com/artifact/org.osgi/osgi.annotation/8.1.0)
  * [OSGi Compendium Spec for Logging 1.5](https://central.sonatype.com/artifact/org.osgi/org.osgi.service.log/1.5.0)
  * more to add here...

## OSGi and C/C++

* Apache Celix project
  * <https://celix.apache.org/>

* <https://blog.cppmicroservices.org/2012/03/29/osgi-and-c++/>

* nOSGi - Native OSGi implementation, Universität Ulm (development stopped in 2014)
  * <https://sourceforge.net/p/nosgi/home/Home/>
  * <https://dl.acm.org/doi/10.1145/2016551.2016555>
  * <https://www.uni-ulm.de/in/in/vs/res/proj/nostrum/getting-started/>

## C++ Language

* C++ Language reference
  * <https://cppreference.com>
  * Modern C++ from Rainer Grimm: <https://www.modernescpp.com/index.php/c-23-the-next-c-standard>

* C++ Core Guidelines
  * Documentation: <https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines>
  * Guidelines Project: <https://github.com/isocpp/CppCoreGuidelines>
  * Guidelines Support Library: <https://github.com/microsoft/gsl>

* C++ Modules
  * Supported since C++20, better support of STL since C++23
  * Good article series from Rainer Grimm: <https://www.modernescpp.com/index.php/tag/modules>
  * Good blog posts from [Victor Zverovich](https://www.zverovich.net/), maintainer of [@fmtlib](https://github.com/fmtlib)
    * <https://www.zverovich.net/2023/04/10/cxx20-modules-in-clang.html>
    * <https://www.zverovich.net/2023/04/17/simple-cxx20-modules.html>
  * Clang reference about Modules:
    * <https://clang.llvm.org/docs/StandardCPlusPlusModules.html>
  * Gcc reference about Modules:
    * <https://gcc.gnu.org/wiki/cxx-modules>
    * <https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Modules.html>
  * Known issues:
    * clang: [Import std lib as modules with clang](https://stackoverflow.com/questions/66411157/import-std-lib-as-modules-with-clang)
    * clang: [Error when importing "string" and "iostream" in same file with c++-20 standard modules'](https://github.com/llvm/llvm-project/issues/58540)
  * Real World Use Cases
    * MS-Office is using C++-Modules: <https://devblogs.microsoft.com/cppblog/integrating-c-header-units-into-office-using-msvc-1-n/>
    * Moving a project to C++ named Modules: <https://devblogs.microsoft.com/cppblog/moving-a-project-to-cpp-named-modules/>

* Multiple inheritance
  * <https://stackoverflow.com/questions/48407658/stumped-by-simple-c-multiple-inheritance-example>

## C++ Libraries

* C++ Libraries
  * <https://github.com/google?language=c%2B%2B>
  * <https://blog.codacy.com/10-cpp-open-source-projects/>

* Testing
  * <https://github.com/google/googletest>

* Fuzzy testing
  * <https://github.com/CodeIntelligenceTesting/cifuzz>

* Logging frameworks
  * <https://github.com/SergiusTheBest/plog> ==> selected logging framework
  * <https://github.com/gabime/spdlog>
  * <https://logging.apache.org/log4cxx/latest_stable/>

* Reflection
  * <https://www.rttr.org/>
  * <https://bitbucket.org/barczpe/oops/src/master/>
  * meta: Header-only runtime reflection system in C++
    * <https://github.com/skypjack/meta/>
    * ==> looks very promising
    * Good build system
  * Describe: A C++14 Reflection Library
    * <https://www.boost.org/doc/libs/develop/libs/describe/doc/html/describe.html>
  * meta: C++20 tiny dynamic reflection library
    * <https://github.com/BlackMATov/meta.hpp>
  * refl-cpp: C++20 tiny dynamic reflection library
    * <https://github.com/veselink1/refl-cpp>
    * <https://veselink1.github.io/refl-cpp/index.html>
  * Reflection TS: The Reflection TS is based on the C++20 standard
    * <https://en.cppreference.com/w/cpp/experimental/reflect>
  * MetaReflect: Metareflect is a lightweight reflection system for C++, based on LLVM and Clangs libtooling.
    * <https://github.com/Leandros/metareflect>

* Databases
  * RocksDB: A Persistent Key-Value Store for Flash and RAM Storage
    * <https://github.com/facebook/rocksdb>
    * Evolved from LevelDB
  * JunoDB: JunoDB - A secure, consistent and highly available key-value store
    * <https://github.com/paypal/junodb>
    * relies on RocksDB

* Telnet C++ libraries
  * libtelnet: libtelnet is a lightweight Telnet protocol handling library written in C. It can be easily used in C++ projects
    * <https://github.com/seanmiddleditch/libtelnet>
  * TelnetServLib: TelnetServLib is a very light ANSI Telnet Server library
    * <https://github.com/lukemalcolm/TelnetServLib>
    * builtin telnet server with callbacks
  * Telnet++: Telnet++ is an implementation of the Telnet Session Layer protocol that is used primarily to negotiate a feature set between a client and server, the former of which is usually some kind of text-based terminal, Commonly used terminals include Xterm, PuTTY, and a whole host of Telnet-enabled MUD clients including Tintin++, MushClient, and more.
    * <https://github.com/KazDragon/telnetpp>
    * sounds most promising implementation, will give it a try
  * Boost.Asio: Boost.Asio is a C++ library for network programming.
    * <https://www.boost.org/doc/libs/1_82_0/doc/html/boost_asio.html>
    * no builtin telnet server

## C++ Projects

* Interesting projects
  * ENTT: EnTT is a header-only, tiny and easy to use library for game programming and much more written in modern C++.
    * <https://github.com/skypjack/entt>

## C++ Tooling

* Google C++ Style guide
  * <https://google.github.io/styleguide/cppguide.html>
  * <https://github.com/google/styleguide>
  * <https://opensource.google/documentation/policies/cplusplus-support>

* Linter
  * cpplint (by Google): <https://github.com/google/styleguide/blob/gh-pages/cpplint/cpplint.py>
  * Maintained version: <https://github.com/cpplint/cpplint>
  * <https://stackoverflow.com/questions/51582604/how-to-use-cpplint-code-style-checking-with-cmake>
  * Good video, recommended to view: <https://www.youtube.com/watch?v=rLopVhns4Zs&t=4633s>
  * <https://include-what-you-use.org/>
  * <https://stackoverflow.com/questions/45667850/disable-specific-warnings-from-cpplint>

* Linter hints
  * "explicit" should be used on single-parameter constructors and conversion operators: <https://rules.sonarsource.com/cpp/RSPEC-1709>
  * Google Style Guide `<chrono>` is an unapproved C++11 header": <https://stackoverflow.com/questions/33653326/google-style-guide-chrono-is-an-unapproved-c11-header>

* IDE's
  * VisualStudio Code: <https://code.visualstudio.com/>
    * C++ extensions installed:
      * Bazel
      * C/C++ IntelliSense
      * Clang-Format
      * CMake, CMake Tools
      * Makefile Tools
  * JetBrains CLion: <https://www.jetbrains.com/clion/>
    * More C++ tooling: <https://www.jetbrains.com/cpp/>

## C++ CI/CD build systems

* CMake
  * <https://code.visualstudio.com/docs/cpp/cmake-linux>
  * <https://cmake.org/cmake/help/latest/guide/tutorial/index.html>
  * <https://github.com/zemasoft/clangformat-cmake>
  * <https://github.com/cheshirekow/cmake_format>
  * <http://google.github.io/googletest/quickstart-cmake.html>
  * C++ Modules
    * Cmake and C++ Modules: <https://www.kitware.com/import-cmake-c20-modules/>
    * [Simple usage of C++20 modules](https://www.zverovich.net/2023/04/17/simple-cxx20-modules.html), gives an alternative from [Victor Zverovich](https://www.zverovich.net/)
    * [C++20 Modules support for CMake](https://www.reddit.com/r/cpp/comments/1255t31/c20_modules_support_for_cmake/), talk for CppCon 2023 announced, but not much progress expected
    * Sample at Godbolt: [hello sample](https://godbolt.org/z/aTr8crhcE)

* Bazel
  * Bazel Build System for Embedded Projects: <https://interrupt.memfault.com/blog/bazel-build-system-for-embedded-projects>, good article, worth reading
  * Common C++ Build Use Cases: <https://bazel.build/tutorials/cpp-use-cases>
  * Bazel and C++ Modules:
    * <https://buildingblock.ai/cpp20-modules-bazel>
    * <https://github.com/rnburn/cpp20-module-example/tree/main>
    * Bazel issue for [Support C++20 modules](https://github.com/bazelbuild/bazel/issues/4005)

* xmake
  * Project: <https://github.com/xmake-io/xmake>
  * [Xmake v2.7.1 Released, Better C++ Modules Support · xmake-io/xmake Wiki](https://www.reddit.com/r/cpp/comments/wy87wv/xmake_v271_released_better_c_modules_support/), high-level comparison of Meson/build2/Bazel/Buck/xmake
  * [C++ Module samples](https://github.com/xmake-io/xmake/tree/master/tests%2Fprojects%2Fc%2B%2B%2Fmodules)
