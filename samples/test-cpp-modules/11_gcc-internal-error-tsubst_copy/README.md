# Submit GCC Bug

## Bug submission

* Bug reported: <https://gcc.gnu.org/bugzilla/show_bug.cgi?id=110870>

```plain
Title: g++ internal compiler error on modules with precompiled headerfile "string"
Product: gcc
Comp: c++

// Info
gcc-13 --version
gcc-13 (Homebrew GCC 13.1.0) 13.1.0
macOS 13.5, arm64, Apple M1 MAX

// Steps to reproduce
* use C++20 modules with precompiled header string
* gcc 13.1 reports: "internal compiler error: in tsubst_copy, at cp/pt.cc:17275"
* happens with plog-1.1.19, but seems unrelated

// i.cc
export module i;

// m_impl.cc
module;
#include <plog/Log.h>
module m:impl;
import i;

// m_public.c
export module m:_public;

import i;
import :impl;

// m.cc
export module m;
export import :_public;

// build
gcc-13 -std=c++20 -xc++-system-header -fmodule-header /opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/string
gcc-13 -std=c++20 -fmodules-ts -Wall -g -MMD -I../../../third_party/plog-1.1.9/include -c i.cc -o i.o
gcc-13 -std=c++20 -fmodules-ts -Wall -g -MMD -I../../../third_party/plog-1.1.9/include -c m_impl.cc -o m_impl.o
gcc-13 -std=c++20 -fmodules-ts -Wall -g -MMD -I../../../third_party/plog-1.1.9/include -c m_public.cc -o m_public.o
gcc-13 -std=c++20 -fmodules-ts -Wall -g -MMD -I../../../third_party/plog-1.1.9/include -freport-bug -c m.cc -o m.o
In file included from /opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/bits/locale_classes.h:869,
                 from /opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/bits/ios_base.h:41,
                 from /opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/ios:44,
                 from /opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/istream:40,
                 from /opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/sstream:40,
                 from ../../../third_party/plog-1.1.9/include/plog/Util.h:6,
                 from ../../../third_party/plog-1.1.9/include/plog/Record.h:4,
                 from ../../../third_party/plog-1.1.9/include/plog/Appenders/IAppender.h:2,
                 from ../../../third_party/plog-1.1.9/include/plog/Logger.h:2,
                 from ../../../third_party/plog-1.1.9/include/plog/Log.h:7,
                 from m_impl.cc:3,
of module m:impl, imported at m_public.cc:4,
of module m:_public, imported at m.cc:3:
/opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/bits/locale_classes.tcc: In instantiation of 'const _Facet& std::use_facet(const locale&) [with _Facet = __cxx11::numpunct@m:impl<char>]':
/opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/bits/locale_facets.tcc:80:66:   required from 'void std::__numpunct_cache@m:impl<_CharT>::_M_cache(const std::locale&) [with _CharT = char]'
/opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/bits/locale_facets.tcc:63:18:   required from 'const std::__numpunct_cache@m:impl<_CharT>* std::__use_cache@m:impl<std::__numpunct_cache@m:impl<_CharT> >::operator()(const std::locale&) const [with _CharT = char]'
/opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/bits/locale_facets.tcc:384:33:   required from '_InIter std::num_get@m:impl<_CharT, _InIter>::_M_extract_int(_InIter, _InIter, std::ios_base@m:impl&, std::ios_base@m:impl::iostate&, _ValueT&) const [with _ValueT = long int; _CharT = char; _InIter = std::istreambuf_iterator<char, std::char_traits@m:impl<char> >; std::ios_base@m:impl::iostate = std::ios_base@m:impl::iostate]'
/opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/bits/locale_facets.h:2225:30:   required from 'std::num_get@m:impl<_CharT, _InIter>::iter_type std::num_get@m:impl<_CharT, _InIter>::do_get(iter_type, iter_type, std::ios_base@m:impl&, std::ios_base@m:impl::iostate&, long int&) const [with _CharT = char; _InIter = std::istreambuf_iterator<char, std::char_traits@m:impl<char> >; iter_type = std::istreambuf_iterator<char, std::char_traits@m:impl<char> >; std::ios_base@m:impl::iostate = std::ios_base@m:impl::iostate]'
/opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/bits/locale_facets.h:2223:7:   required from here
/opt/homebrew/Cellar/gcc/13.1.0/include/c++/13/bits/locale_classes.tcc:203:60: internal compiler error: in tsubst_copy, at cp/pt.cc:17275
  203 |       if (const _Facet* __f = std::__try_use_facet<_Facet>(__loc))
      |                                                            ^~~~~
Please submit a full bug report, with preprocessed source.
See <https://github.com/Homebrew/homebrew-core/issues> for instructions.
Preprocessed source stored into /var/folders/_f/yfc6xzvd2sd1gsvthzlfkbdm0000gn/T//ccPMd0Iz.out file, please attach this to your bugreport.
make: *** [gcc] Error 1

# see attached output file cczqIydL.out.txt

// m.cc -MDD
m.o gcm.cache/m.gcm: m.cc
m.o gcm.cache/m.gcm: m:_public.c++m
m.c++m: gcm.cache/m.gcm
.PHONY: m.c++m
gcm.cache/m.gcm:| m.o
CXX_IMPORTS += m:_public.c++m

// notes
* internal compiler error does not happen, when string will NOT be precompiled
* internal compiler error does not happen, when m.cc does NOT import m:public
```
