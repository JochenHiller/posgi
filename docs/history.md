# History of knowledge and experiences in past

This project `posgi` (OSGi in C++) has primarily been started by me as a self education project. I had some experiences in past with C and C++, which I wanted to update to latest programming language evolutions of the last 10-15 years.

## C/C++ History

During my studies around 1988, I used C language in real time operating systems like [QNX](https://en.wikipedia.org/wiki/QNX) and [OS/9](https://en.wikipedia.org/wiki/OS-9). Dissatisified with the traditional software development, I searched for new software engineering concepts and stumbled over Object-oriented software development. I ordered in US a [Zortech C++ compiler](https://en.wikipedia.org/wiki/Digital_Mars), C-Scape UI library and [Digitalk Smalltalk/V](https://winworldpc.com/product/digital-smalltalk/5x) for PC 286. I had to develop graphical applications in health care (some years before Windows entered the market). Smalltalk was too inefficient for applications these times, and the Zortech C++ compiler was so buggy and non-debuggable, that I backported C++ to C again to get the application finally working.

In 1992 I had the opportunity to work on a NeXT workstation. I had to develop a hardware driver, so I decided to stick on C, and do not use Objective-C, which was quite new this time. [Objective-C](https://en.wikipedia.org/wiki/Objective-C) uses some Smalltalk concepts (message passing), but added that to a procedural language. Finally I worked same year on a graphical configuration tool for high-scalable hardware (Transputer, DSPs), where I used [InterViews](https://www.researchgate.net/publication/2953839_Composing_User_Interfaces_with_InterViews) as UI framework on X-Windows and [OSF/Motif](https://en.wikipedia.org/wiki/Motif_(software)). InterViews was from Stanford University, one of the authors was John Vlissides, also Co-author of the famous [Design Patterns](https://en.wikipedia.org/wiki/Design_Patterns) book. I had to get the GCC 2.2 compiler from University on tape and had to compile it for myself on SUN Solaris and Interactive/Unix to use it for UI development.

After working 4 years in business with an incredibale Smalltalk setup ([Objectory](https://en.wikipedia.org/wiki/Objectory_AB), [VisualWorks/Smalltalk-80](https://wiki.c2.com/?VisualWorksSmalltalk), [HP Distributed Smalltalk](https://www.hpl.hp.com/hpjournal/95apr/apr95a11.pdf) and [ENVY/Developer](https://wiki.c2.com/?EnvyDeveloper)), which was a great time, I lead again a C++ project with a small team. We used [SNIFF+](https://techmonitor.ai/technology/takefive_has_sniff_c_and_c_development_environment) and the customer C++ framework for a critical banking application. SNIFF+ was a first reasonable IDE for C++ (like we knew it from Smalltalk), and not only a simple editor.

1999 we developed parts of an Enterprise Service Bus, publishing metadata of banking products on ESB, in Java and C++. We learned that Java development was much more efficient, so we developed first in Java, and backported then the code to C++. The ESB was later partly published as OpenSource project [openadaptor](https://github.com/openadaptor).

After being 25 years now a Java developer, I stumbled again over C++ when we are integrating the new [Matter protocol](https://en.wikipedia.org/wiki/Matter_(standard)) in a commercial SmartHome product I am partly responsible for. For Matter, there is an [OpenSource C++ implementation](https://github.com/project-chip/connectedhomeip) which can be integrated into SmartHome gateways (like Apple HomePod, Google NEST) or on embedded devices (like smart plugs, window contacts, heating thermostats and many more) from different hardware vendors. This finally convinced me to update my old C++ skills to the latest versions.

## OSGi History

In year 2003 I stumbled over OSGi specification, and an OpenSource implementation of it named [Oscar](https://ieeexplore.ieee.org/document/1286894) (developed by Richard Hall). It was quite interesting, especially its concept of dynamic applications, which can be extended and updated during runtime, which makes it a great solution for embedded systems.

At the same time, I was a user of Eclipse IDE 2.x for Java development, first time that an IDE came close to my experiences in past when developing with Smalltalk. I also wrote plugins for Eclipse, e.g. an small tool to compile and run Java code on [leJOS](https://lejos.sourceforge.io/index.php), a tidy JVM for [Lego Mindstorms RCX](https://en.wikipedia.org/wiki/Lego_Mindstorms#RCX). Eclipse decided to change its underlying plugin concept to be OSGi based from 3.0 on, developing its own OSGi implementation named [Equinox](https://www.eclipse.org/equinox/) (core developers have been Jeff McAffer and Thomas Watson from IBM).
As plugin developer, it was quite interesting to see the migration during 3.0 release phase finally reaching a plain OSGi framework and keeping compatibility to old plugins. Since that time I was somehow _infected_ by OSGi and its concepts.

In daily business I was doing JavaEE based web development at this time. I extended our architecture to modularize the application based on ideas of OSGi, using class loader hierachies and web application class loaders mirroring the module concept. And I always thought about how web applications could be integrated better with the OSGi bundle concept. 2005 there was a breakthrough by Simon Kaegi from Cognos/later IBM, who made OSGi able to run inside a web container. There have been much more progress the next years, about a more easy way to use OSGi inside a web app, or add web applications to an OSGi framework by new OSGi specifications.

Togother with friends Simon Kaegi, Gunnar Wagenknecht, Frank Gerhardt I gave some tutorials ([1](https://www.slideshare.net/mfrancis/building-serverside-eclipse-based-web-applications-jochen-hiller-principal-architect-business-operation-systems-germany), [2](https://preview.jugsaxony.org/downloads/events//Building-Server-Applications-With-EclipseRT.pdf)) and presentations ([3](https://www.slideshare.net/mfrancis/modular-web-applications-based-on-osgi-jochen-hiller)) at many EclipseCon conferences to convince people about OSGi. In business when I started a new job, I tried as well to introduce OSGi in new applications. Some cloud applications have been developed using OSGi, modularization has been used to define common interfaces, and split implementations between multiple software suppliers.

Finally end of 2011 my employer started a new platform [QIVICON](https://www.slideshare.net/jochen.hiller/osgi-users-forum-germay201404qivicon) for a SmartHome product. A general platform decision was to rely on OSGi as core technology for embedded devices with dynamic applications and dynamic updates. Now I was heavily back in OSGi world, using [commercial implementations](https://en.wikipedia.org/wiki/ProSyst), [moving over](https://www.eclipsecon.org/europe2018/sessions/migrating-pde-bndtools-practice) to OpenSource implementations ([Apache Felix](https://felix.apache.org/documentation/index.html), which was success of Oscar). I do not know exactly, but I assume this rollout of OSGi to embedded devices (routers) and its dynamic update management through the cloud is maybe one of the biggest rollouts of OSGi to embedded systems ever.
[We are also using openHAB](https://www.openhabfoundation.org/documents/2017-10_Jochen_Hiller_QIVICON.pdf), an OpenSource based solution relying as well on OSGi (Equinox, Apache Karaf). I did an integration of openHAB with [Eclipse Concierge](https://www.slideshare.net/mfrancis/modular-web-applications-based-on-osgi-jochen-hiller), another tiny OSGi implementation (by Jan Rellermeyer and Tim Verbelen). Accidentally I was nominated as committer as I took care about the implementation for some time. Meanwhile [Eclipse Concierge](https://github.com/eclipse-archived/concierge) has been archived due to lack of time of us committers.

## Why developing posgi: an C++ based OSGi implementation

When thinking about a C++ project, I wanted to at least parts rely on existing knowledge. OSGi is well-known for its clear specification, its modular approach, its modular compendium bundles adding features by bundles. I thought about, why not re-implementing OSGi in C++, knowing that the different language concepts between Java and C++ will somehow limit the C++ features.

* OSGi has clear specifications
  * OSGi has clear separation between API and implementations
* OSGi has enough complexity for a real world challenge
  * Multithreading needed
  * Shared memory management beween framework, bundles, application
  * RTTI and Reflection can be used
* Use standard development approach to reach good quality
  * Use standard libraries
  * Use standard code style guides and source code formatting
  * Add unit testing, integration testing, fuzz testing
  * Use linters for good code
  * Use reliable CI/CD process (e.g. CMake, Bazel)
* Finally: make everything OpenSource

There are as well other OSGi C/C++ implementations available, which I simply ignored for the moment ([nosgi](https://sourceforge.net/p/nosgi/home/Home/) stopped development in 2014, [Apache Celix](https://celix.apache.org/) is mainly C-based, did not fit my expectations).

I also thought about trying out [Carbon](https://github.com/carbon-language/carbon-lang), the new proposed language by Google. Carbon might be a successor of C++ in future, but at the moment the project is not mature enough to build real applications with it. Nevertheless I will have an eye on Carbon, because many of the concerns I had and learned again about C++ will be addressed by Carbon.

Let's see how far `posgi` will evolve as a spare time project.
