# Project plan of posgi: OSGi in C++

## Roadmap

### Minimum viable product (MVP)

* Install bundles from existing binary
* Start and and stop bundles
* Parse MANIFEST.MF
* Console bundle to manage framework and bundles (stdin/stdout console)
* Bundle tracker
* Common logging, prepare for later log service
* Namespaces: osgi::framework, osgi::services ???

### Further features (backlog)

* Service registry
* SCR, OSGI-INF/*.xml
* GCC, LLVM compiler support
* CMake, Bazel build systems
* Add standard OpenSource files to project
* Use C++ linter
* Automatic code formatting of C++ code (`.h`, `.cc`)

### Out-of-Scope

* dynamic loading of bundles (as shared libraries)
  * packaging of bundles (as ZIP)
