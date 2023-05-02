# Implementation of OSGi in C++

## Overview

This project is a first, simple C++ based implementation of OSGi. It is intended to refresh my old C++ skills with latest C++ versions.

It will be limited to core features of OSGi R8, which are applicable to be implemented in C++ as well.

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

### OSGi specifications

* https://docs.osgi.org/specification/
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
  * https://celix.apache.org/

* https://blog.cppmicroservices.org/2012/03/29/osgi-and-c++/

* nOSGi - Native OSGi implementation, Universität Ulm (developed until 2014)
  * https://sourceforge.net/p/nosgi/home/Home/
  * https://dl.acm.org/doi/10.1145/2016551.2016555
  * https://www.uni-ulm.de/in/in/vs/res/proj/nostrum/getting-started/

### C++

* https://github.com/google?language=c%2B%2B
* https://blog.codacy.com/10-cpp-open-source-projects/

* Google C++ Style guide
  * https://google.github.io/styleguide/cppguide.html
  * https://google.github.io/styleguide/cppguide.html
