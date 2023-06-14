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

## Fuzzy testing

For fuzzy testing we use [cifuzz](https://github.com/CodeIntelligenceTesting/cifuzz), an interesting fuzzy testing tool from CodeIntelligence. We do NOT use their central CI server, we just do local fuzz test generation.

It fits well with a C/C++ program as it

* integrates very well with CMake
* It can create corpus data based on reasonable input
* It will create input files for failed tests for regression testing
* It has source code instrumentation to observe code coverage, using `lcov`
* It can create nice HTML reports about code coverage
* It integrates as well with VS code and CLion IDEs to show code coverage (not yet tested)

The setup is quite easy:

```bash
# initialize your project
./third_party/cifuzz/bin/cifuzz init

# create a simple fuzz test to start from
./third_party/cifuzz/bin/cifuzz create

# run fuzz tests indefinitely, use corpus with good input data, stop wth CTRL-C
./third_party/cifuzz/bin/cifuzz run \           
  --interactive=false \
  --seed-corpus framework/impl/posgi_fuzz_tests_manifest_inputs \
  posgi_fuzz_tests_manifest

# generate a HTML report of fuzz tests
./third_party/cifuzz/bin/cifuzz coverage --output=./build/coverage-report posgi_fuzz_tests_manifest
```

There are two fuzz tests for parsing manifest file, checking log formatter. Up to now no problems have been identified, so code seems to be OK and not breakable by unexpected data. For the test implementation see [manifest_parser_fuzz_test.cc](../framework/impl/manifest_parser_fuzz_test.cc).

Note: the fuzz tests have to be started by their own (when running fuzz tests only local). There are way to run fuzz tests with

```bash
./build.sh fuzztest-run-01   # for manifest
./build.sh fuzztest-run-02   # for txtformatter
```

## Commits

We try to follow [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/). We do also experiment with some AI assitance in generating useful commit messages.

## C++ Pros and Cons

### Pros

* Multi-line string literals supported since C++11 (see [here](https://en.cppreference.com/w/cpp/language/string_literal))
* Googletest is very useful testing framework
  * easy writing test cases, easy test suites, CMake integration
  * ignore tests, parameterized tests, run single tests on CLI if needed
  * hides console output, can be enabled by start parameter (`--gtest_catch_exceptions=0`)
* Google style guide recommends to avoid to use exceptions, which I support
* Modern C++ supports Embedded systems very good
  * C++14: Binary literals, e.g. `0b1010'1100`
  * C++17: enforce processing of return values with `[[nodiscard]]` (like in Ada)
  * C++17: `static_assert()` mechanism, `if constexpr` for compile time optimizations
  * C++17: guaranteeing copy elision (return value optimization), for hard real-time systems

### Cons

* Separation of header (`.h`) and implementation (`.cc`) files very inefficient. Leads to a lot of code-duplication and manual maintenance, which could be done by tooling in a better way (like in [Golang](https://go.dev/), or proposed for [Carbon](https://github.com/carbon-language/carbon-lang))
* String format: old plain C-style vs. modern C++ style: see [osgi_console.cc](../framework/osgi_console.cc): both options are not really nice. Using old C-style requires conversion from `char *` to `std::string` and vice versa. And the C++ formatting options are also not as intuitive.
* Googletest framework does complain with linter(s)

### Header-only C++ libraries

Some usthird party C++ libraries used are so called "Header-only libraries" (e.g. plog). We checked if this would make sense as well for posgi, but due to drawbacks of such a solution we decided to **NOT** implement posgi as header-only approach. Especially the unclear separation between API and implementation was a clear argument against header-only. criteria.

There is a good discussion about Pros and Cons of this approach [here](https://stackoverflow.com/questions/12671383/benefits-of-header-only-libraries).
