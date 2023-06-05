#ifndef POSGI_ORG_OSGI_FRAMEWORK_CONSTANTS_H
#define POSGI_ORG_OSGI_FRAMEWORK_CONSTANTS_H

namespace osgi {

class Constants {
 public:
  // inline supported since C++17:
  // https://en.cppreference.com/w/cpp/language/inline
  inline static const char kBundleActivator[] = "Bundle-Activator";
  inline static const char kBundleSymbolicName[] = "Bundle-SymbolicName";
  inline static const char kBundleVersion[] = "Bundle-Version";
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_FRAMEWORK_CONSTANTS_H
