# Implementation of posgi: OSGi in C++

## Source Code style

I think in modern times we should not discuss about source code style anymore. I find it very boring, to do manual source code formatting. I like the way how Golang does work: the Go code will be automatically formatted by the tooling.

To rely on common used code styles, I am using the Google C++ style guide, and associated toolset:

* format C++ code (`.h`, `.cc`) with clang-format on CLI
* use Clang-Format extension inside VisualStudio code as default formatter. Use Google style for formatting.
* Use "Auto-format on Save" inside IDE

Up-to-now I am using the Google style guide without changes, for simplicity.

## Other source code conventions used

* Guards in header files: <https://google.github.io/styleguide/cppguide.html#The__define_Guard>
  * no `#pragma once`
* Filename: snake case with underscore `_`, ends with `.cc`. That is even uncommon for a 25 years Java programmer, but based on Internet research I am trying to use snake case and not the traditional CamelCase like used in Java.
  * see <https://google.github.io/styleguide/cppguide.html#File_Names>
  * see <https://stackoverflow.com/questions/32434294/camelcase-vs-snake-case-in-c-c-identifiers-and-filenames>
  * see <http://www.cs.kent.edu/~jmaletic/papers/ICPC2010-CamelCaseUnderScoreClouds.pdf>
* Sort header files in well defined order
  * Use double quotes `".../*.h"` for non standard library header files
  * <https://google.github.io/styleguide/cppguide.html#Names_and_Order_of_Includes>
* Use namespaces
  * See <https://google.github.io/styleguide/cppguide.html#Namespaces>
  * See tips from [Abseil](https://abseil.io/tips/130) as well

## C++ Pros and Cons

### Pros

* Multi-line string literals supported since C++11 (see [here](https://en.cppreference.com/w/cpp/language/string_literal))

### Cons

* Separation of header (`.h`) and implementation (`.cc`) files very inefficient. Leads to a lot of code-duplication and manual maintenance, which could be done by tooling in a better way (like in [Golang](https://go.dev/), or proposed for [Carbon](https://github.com/carbon-language/carbon-lang))
* String format: old plain C-style vs. modern C++ style: see [osgi_console.cc](../framework/osgi_console.cc): both options are not really nice. Using old C-style requires conversion from `char *` to `std::string` and vice versa. And the C++ formatting options are also not as intuitive.

## References

### OSGi specifications

* <https://docs.osgi.org/specification/>
* OSGi R8 - Specification
  * [OSGi Core Release 8 Specification](https://docs.osgi.org/specification/osgi.core/8.0.0/) ([pdf](https://docs.osgi.org/download/r8/osgi.core-8.0.0.pdf))
  * [OSGi Compendium Release 8 Specification](https://docs.osgi.org/specification/osgi.cmpn/8.0.0/) ([pdf](https://docs.osgi.org/download/r8/osgi.cmpn-8.0.0.pdf))
  * [OSGi Compendium Release 8.1 Specification](https://docs.osgi.org/specification/osgi.cmpn/8.1.0/) ([pdf](https://docs.osgi.org/download/r8/osgi.cmpn-8.1.0.pdf))
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

### OSGi and C/C++

* Apache Celix project
  * <https://celix.apache.org/>

* <https://blog.cppmicroservices.org/2012/03/29/osgi-and-c++/>

* nOSGi - Native OSGi implementation, Universität Ulm (development stopped in 2014)
  * <https://sourceforge.net/p/nosgi/home/Home/>
  * <https://dl.acm.org/doi/10.1145/2016551.2016555>
  * <https://www.uni-ulm.de/in/in/vs/res/proj/nostrum/getting-started/>

### C++

* C++ Language reference
  * <https://cppreference.com>

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

* Linter
  * cpplint (by Google): <https://github.com/google/styleguide/blob/gh-pages/cpplint/cpplint.py>
  * Maintained version: <https://github.com/cpplint/cpplint>
  * <https://stackoverflow.com/questions/51582604/how-to-use-cpplint-code-style-checking-with-cmake>
  * Good video, recommended to view: <https://www.youtube.com/watch?v=rLopVhns4Zs&t=4633s>
  * <https://include-what-you-use.org/>
  * <https://stackoverflow.com/questions/45667850/disable-specific-warnings-from-cpplint>

* Linter hints
  * "explicit" should be used on single-parameter constructors and conversion operators: <https://rules.sonarsource.com/cpp/RSPEC-1709>
  * Google Style Guide "<chrono> is an unapproved C++11 header": <https://stackoverflow.com/questions/33653326/google-style-guide-chrono-is-an-unapproved-c11-header>

* Multiple inheritance
  * <https://stackoverflow.com/questions/48407658/stumped-by-simple-c-multiple-inheritance-example>

* Testing
  * <https://github.com/google/googletest>

* Logging frameworks
  * <https://github.com/SergiusTheBest/plog> ==> selected logging framework
  * <https://github.com/gabime/spdlog>
  * <https://logging.apache.org/log4cxx/latest_stable/>

* Reflection
  * <https://www.rttr.org/>
  * <https://bitbucket.org/barczpe/oops/src/master/>
