# Project plan of posgi: OSGi implementation in C++

## Roadmap

### Work in progress (WIP)

* [ ] Improve IDEs (VS Code, CLion)
* [ ] console as service bundle
  * [ ] bundles/console/include/org/osgi/service/console/console.h, command.h
  * [ ] bundles/console/impl/osgi_console.cc, command_xxx.cc
* [ ] Pros and Cons C++
* [ ] check for smart memory management
* [ ] Linux build and tests
* [ ] Coverage: fuzz, exlucde third_party libraries (plog, boost)

### Minimum viable product (MVP)

* [ ] OSGi Framework: Core APIs
* [ ] OSGi Bundle Layer
  * [x] Install bundles from linked classes
  * [x] Bundle states
  * [x] MANIFEST Parser
  * [x] Start and stop bundles
  * [ ] Bundle tracker
* [ ] OSGi Compendium Bundles
  * [ ] Console bundle to manage framework and bundles (stdin/stdout console, telnet)
* [ ] Implementation
  * [ ] Common logging, prepare for later log service
  * [x] Log Formatter with this format: limit thread name and class/file to max. size, cut from right side
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

Specification for LogFormatter:

```txt
# <Timestamp (ISO8601)> " " <LogLevel> " " [<[thread-id-or-name]>] [<file-class-method@line>] [msg]
# - [<[thread-id-or-name]>] shortened to max 15 chars
# - [<file-class-method@line>] shortened to max 25 chars
2023-05-18T12:41:14.082Z ERROR [thread-id-or-name] [posgi...meworkThreadLoop@186] FrameworkImpl::frameworkThreadLoop: Signal received!
```

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
