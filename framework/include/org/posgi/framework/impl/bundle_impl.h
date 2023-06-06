#ifndef POSGI_ORG_POSGI_FRAMEWORK_IMPL_BUNDLE_IMPL_H
#define POSGI_ORG_POSGI_FRAMEWORK_IMPL_BUNDLE_IMPL_H

#include "org/osgi/framework/bundle.h"
#include "org/osgi/framework/bundle_context.h"
#include "org/posgi/framework/impl/bundle_context_impl.h"
// #include "org/posgi/framework/impl/framework_impl.h"

namespace posgi {

class BundleContextImpl;

class BundleImpl : public virtual osgi::Bundle {
 public:
  void Start();
  void Stop();
  int GetId();
  std::string GetSymbolicName();
  osgi::BundleContext *GetBundleContext();
  void SetBundleContext(BundleContextImpl *bundleContextImpl);
  int GetState();
  void SetState(int state);
  std::string GetStateAsString();

  // TODO(jhi): make all private with setters, or use friends
  BundleContextImpl *bc;
  int id;
  std::string bundleSymbolicName;
  osgi::BundleActivator *activatorRef = nullptr;

 private:
  std::string getStateAsStringMapping(int state);
  std::string to_string();
  int state = UNINSTALLED;
};

}  // end namespace posgi

#endif  // POSGI_ORG_POSGI_FRAMEWORK_IMPL_BUNDLE_IMPL_H
