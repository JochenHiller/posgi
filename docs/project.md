# Project plan of posgi: OSGi implementation in C++

## Roadmap

### Work in progress (WIP)

* [x] Copy code from sample02 to framework
* [x] Start/stop bundles
* [ ] Bundles with bundle state
* [ ] CMake build
* [ ] C++ Linter
* [ ] Pros and Cons C++
* [ ] Linux build and tests

### Minimum viable product (MVP)

* [ ] OSGi Framework: Core APIs
* [ ] OSGi Bundle Layer
  * [ ] Install bundles from linked classes
  * [ ] MANIFEST Parser
  * [ ] Start and stop bundles
  * [ ] Bundle tracker
* [ ] OSGi Compendium Bundles
  * [ ] Console bundle to manage framework and bundles (stdin/stdout console)
* [ ] Implementation
  * [ ] Common logging, prepare for later log service
  * [ ] Log Formatter with this format: limit thread name and class/file to max. size, cut from right side
  * [ ] Namespaces: osgi::framework, osgi::services ???
  * [ ] Named threads
* [ ] CI/CD
  * [ ] Clang C++17
  * [ ] CMake build
  * [ ] libposgi-0.1.0.so delivery
  * [ ] C++ Linter integrated
  * [ ] Auto-Format (IDE, build) (`.h`, `.cc`)
  * [ ] Doxygen generated docs, incl. class diagram
  * [ ] Docs how to build and run on Linux (using Docker)
  * [ ] Try to use VisualStudio Code in cloud IDE (VS Code, GitPod)
* [ ] Documentation
  * [x] Standard OpenSource files in project
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
* [ ] Further Ideas
  * [ ] Check for OSGi.fx support (Bundle/Service layer) (agent, sockets, DTOs)
  * [ ] Gogo telnet console
  * [ ] Experimental HttpService

### Out-of-Scope

As of 06/2023:

* [ ] OSGi Bundle Layer
  * [ ] Install bundles from zip file
    * [ ]  Tooling support to build zip files
