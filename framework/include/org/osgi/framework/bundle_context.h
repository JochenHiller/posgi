#ifndef POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_CONTEXT_H
#define POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_CONTEXT_H

#include <string>

namespace osgi {

// forward declaration needed to dependency of Bundle <-> BundleContext
class Bundle;
// forward declaration needed to dependency of BundleActivator <-> BundleContext
class BundleActivator;

class BundleContext {
 public:
  virtual Bundle *InstallBundle(std::string manifest,
                                BundleActivator *activator = nullptr) = 0;
  virtual Bundle *GetBundle(int bundleId) = 0;
  virtual std::vector<Bundle *> *GetBundles() = 0;
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_CONTEXT_H
