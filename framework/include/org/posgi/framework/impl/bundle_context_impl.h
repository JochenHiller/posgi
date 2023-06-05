#ifndef POSGI_ORG_POSGI_FRAMEWORK_IMPL_BUNDLE_CONTEXT_IMPL_H
#define POSGI_ORG_POSGI_FRAMEWORK_IMPL_BUNDLE_CONTEXT_IMPL_H

#include "org/osgi/framework/bundle.h"
#include "org/osgi/framework/bundle_context.h"

namespace posgi {

class BundleImpl;
class FrameworkImpl;

class BundleContextImpl : public osgi::BundleContext {
 public:
  BundleContextImpl(BundleImpl *bundleImpl);
  BundleContextImpl(FrameworkImpl *frameworkImpl);
  osgi::Bundle *GetBundle(int bundleId);
  std::vector<osgi::Bundle *> *GetBundles();

  osgi::Bundle *InstallBundle(std::string manifest,
                              osgi::BundleActivator *activator = nullptr);

  void dump_bundle_context();
  // TODO(JochenHiller): make private and provide a setter
  FrameworkImpl *frameworkImpl;

 private:
  BundleImpl *bundleImpl;
};

}  // end namespace posgi

#endif  // POSGI_ORG_POSGI_FRAMEWORK_IMPL_BUNDLE_CONTEXT_IMPL_H
