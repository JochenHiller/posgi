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

## C++

* C++ Language reference
  * <https://cppreference.com>

* C++ Core Guidelines
  * Documentation: <https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines>
  * Guidelines Project: <https://github.com/isocpp/CppCoreGuidelines>
  * Guidelines Support Library: <https://github.com/microsoft/gsl>

* C++ Libraries
  * <https://github.com/google?language=c%2B%2B>
  * <https://blog.codacy.com/10-cpp-open-source-projects/>

* Google C++ Style guide
  * <https://google.github.io/styleguide/cppguide.html>
  * <https://github.com/google/styleguide>
  * <https://opensource.google/documentation/policies/cplusplus-support>

* CMake
  * <https://code.visualstudio.com/docs/cpp/cmake-linux>
  * <https://cmake.org/cmake/help/latest/guide/tutorial/index.html>
  * <https://github.com/zemasoft/clangformat-cmake>
  * <https://github.com/cheshirekow/cmake_format>
  * <http://google.github.io/googletest/quickstart-cmake.html>

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

* Multiple inheritance
  * <https://stackoverflow.com/questions/48407658/stumped-by-simple-c-multiple-inheritance-example>

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

* Intersting projects
  * ENTT: EnTT is a header-only, tiny and easy to use library for game programming and much more written in modern C++.
    * <https://github.com/skypjack/entt>

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

* Databases
  * RocksDB: A Persistent Key-Value Store for Flash and RAM Storage
    * <https://github.com/facebook/rocksdb>
    * Evolved from LevelDB
  * JunoDB: JunoDB - A secure, consistent and highly available key-value store
    * <https://github.com/paypal/junodb>
    * relies on RocksDB
