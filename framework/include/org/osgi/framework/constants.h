#ifndef POSGI_ORG_OSGI_FRAMEWORK_CONSTANTS_H
#define POSGI_ORG_OSGI_FRAMEWORK_CONSTANTS_H

namespace osgi {

class Constants {
 public:
  // inline supported since C++17:
  // https://en.cppreference.com/w/cpp/language/inline
  inline static const std::string BUNDLE_ACTIVATOR = "Bundle-Activator";
  inline static const std::string BUNDLE_SYMBOLICNAME = "Bundle-SymbolicName";
  inline static const std::string BUNDLE_VERSION = "Bundle-Version";
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_FRAMEWORK_CONSTANTS_H
