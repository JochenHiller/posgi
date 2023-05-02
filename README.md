# Implementation of OSGi in C++

## Overview

This project is a first, simple C++ based implementation of OSGi. It is intended to refresh my old C++ skills with latest C++ versions.

It will be limited to core features of OSGi, which are applicable to be implemented in C++ as well.

## Roadmap

## Minimum viable product (MVP)

* Install bundles from existing binary
* Start and and stop bundles
* Console bundle to manage framework and bundles (input: stdin/stdout console)
* Bundle tracker

## Further features

* Service registry
* SCR, OSGI-INF/*.xml
* GCC, LLVM compiler support

### Out-of-Scope

* dynamic loading of bundles (as shared libraries)
  * packaging of bundles (as ZIP)

## References

### OSGi

* https://blog.cppmicroservices.org/2012/03/29/osgi-and-c++/

* nOSGi - Native OSGi implementation, Universität Ulm
  * https://sourceforge.net/p/nosgi/home/Home/
  * https://dl.acm.org/doi/10.1145/2016551.2016555
  * https://www.uni-ulm.de/in/in/vs/res/proj/nostrum/getting-started/

### C++

* https://github.com/google?language=c%2B%2B
* https://blog.codacy.com/10-cpp-open-source-projects/
