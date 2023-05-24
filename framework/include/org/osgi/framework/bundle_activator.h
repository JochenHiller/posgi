#ifndef POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_ACTIVATOR_H
#define POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_ACTIVATOR_H

#include "org/osgi/framework/bundle_context.h"

namespace osgi {

class BundleActivator {
 public:
  virtual void Start(BundleContext *bundleContext) = 0;
  virtual void Stop(BundleContext *bundleContext) = 0;
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_ACTIVATOR_H
