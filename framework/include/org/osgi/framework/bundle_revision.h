#ifndef POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_REVISION_H
#define POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_REVISION_H

namespace osgi {

class BundleRevision {
public:
  virtual std::string GetSymbolicName() = 0;
};

} // end namespace osgi

#endif // POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_REVISION_H
