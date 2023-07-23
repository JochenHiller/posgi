# Project plan of posgi: OSGi implementation in C++

## Roadmap

### Work in progress (WIP)

* [ ] Check IF and HOW to use C++ modules
  * [ ] Clang
  * [ ] Gcc
  * [ ] MSVC
  * [ ] CMake, Bazel support
* [ ] Telnet OSGi console, using threadio similar (e.g. thread_locals for cin/cout/cerr redirection)
* [ ] Improve IDEs (VS Code, CLion)
* [ ] Consider to use Conan package manager: <https://conan.io/>
* [ ] Pros and Cons C++
* [ ] check for smart memory management
  * [ ] std::unique_ptr, std::shared_ptr
* [ ] Linux build and tests
* [ ] Coverage: fuzz, exlucde third_party libraries (plog, boost)
* [ ] namespace osgi, osgi::impl oder osgi::internal
* [ ] Samples vs. examples
* [ ] Cmake weniger Output
* [ ] bundle_id_t als int32_t

### Minimum viable product (MVP)

* [ ] OSGi Framework: Core APIs
* [ ] OSGi Bundle Layer
  * [x] Install bundles from linked classes
  * [x] Bundle states
  * [x] MANIFEST Parser
  * [x] Start and stop bundles
  * [ ] Bundle tracker
* [ ] OSGi Compendium Bundles
  * [ ] Console bundle to manage framework and bundles (stdin/stdout console)
    * [ ] Console bundle via telnet
* [ ] Implementation
  * [ ] Common logging, prepare for later log service
  * [x] Log Formatter with format: limit thread name and class/file to max. size, cut from right side
  * [ ] Namespaces: osgi::framework, osgi::services ???
  * [ ] Named threads
* [ ] CI/CD
  * [x] Clang C++17
  * [x] CMake build
  * [x] libposgi-0.1.0.so delivery
  * [x] Core tests of implementation
  * [x] One Fuzz test as a first template for further fuzz tests
  * [x] C++ Linter integrated (cpplint, clang-tidy)
  * [x] Auto-Format (IDE, build) (`.h`, `.cc`)
  * [ ] Doxygen generated docs, incl. class diagram
  * [ ] Docs how to build and run on Linux (using Docker)
  * [ ] Try to use VisualStudio Code in cloud IDE (VS Code, GitPod)
* [ ] Documentation
  * [x] Standard OpenSource files in project
  * [ ] Provide copyright headers, at least class headers, consider to add copyright header by script
  * [ ] Pros and Cons of C++

### Further features (backlog)

* [ ] OSGi Framework
  * [ ] Framework Events
  * [ ] Builtin log service
  * [ ] Persist OSGi storage
* [ ] OSGi Service Layer
  * [ ] Service registry
  * [ ] SCR, OSGI-INF/*.xml
* [ ] OSGi Compendium Bundles
  * [ ] LogService
  * [ ] EventAdmin
* [ ] CI/CD
  * [ ] GitHub Actions pipeline
  * [ ] Unit tests
  * [ ] Integration tests
  * [ ] Fuzzy tests (e.g. on ManifestParser), using Jazzer
  * [ ] G++ 13
  * [ ] Bazel support
  * [ ] Lint: check include files for use, clang-tidy
  * [ ] More C++ Linters (clang-tidy, include-what-you-use, cppcheck)
  * [ ] Coverage: also for normal tests
* [ ] Further Ideas
  * [ ] Check for OSGi.fx support (Bundle/Service layer) (agent, sockets, DTOs), own management agent using socket and JSON?
  * [ ] Gogo telnet console
  * [ ] Experimental HttpService
  * [ ] Benchmark (bundles, services)
* [ ] Platforms (cross-compilation)
  * [ ] x86 32/64 bit, ARM v5/v6/v7 (hardfloat, softfloat)
  * [ ] RISC-V
  * [ ] Others ? ([Zephyr](https://www.zephyrproject.org/), [ROS](https://www.ros.org/), [Arduino)](https://github.com/arduino)

### Out-of-Scope

As of 06/2023:

* [ ] OSGi Bundle Layer
  * [ ] Install bundles from zip file
    * [ ]  Tooling support to build zip files
