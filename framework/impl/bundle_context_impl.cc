#include "org/posgi/framework/impl/bundle_context_impl.h"

#include <string>
#include <vector>

#include "org/osgi/framework/bundle.h"
#include "org/osgi/framework/bundle_context.h"
#include "org/posgi/framework/impl/bundle_context_impl.h"
#include "org/posgi/framework/impl/manifest_parser.h"
#include "plog/Log.h"

namespace posgi {

class FrameworkImpl;

void BundleContextImpl::dump_bundle_context() {
  PLOG_INFO << "BundleContextImpl::dump_bundle_context";
  PLOG_ERROR << "BundleContextImpl::dump_bundle_context " << this;
  PLOG_ERROR << "BundleContextImpl::dump_bundle_context "
             << this->frameworkImpl;
  PLOG_ERROR << "BundleContextImpl::dump_bundle_context " << this->bundleImpl;
}
BundleContextImpl::BundleContextImpl(BundleImpl *bundleImpl) {
  PLOG_INFO << "BundleContextImpl::BundleContextImpl(BundleImpl)";
  this->bundleImpl = bundleImpl;
  this->frameworkImpl =
      nullptr;  // needs to be set during installBundle from FrameworkImpl
}
BundleContextImpl::BundleContextImpl(FrameworkImpl *frameworkImpl) {
  PLOG_INFO << "BundleContextImpl::BundleContextImpl(FrameworkImpl)";
  this->frameworkImpl = frameworkImpl;
  // preserve as well as bundle context
  this->bundleImpl = frameworkImpl;
}
osgi::Bundle *BundleContextImpl::GetBundle(int bundleId) {
  PLOG_INFO << "BundleContextImpl::GetBundle";
  FrameworkImpl *fwImpl = this->frameworkImpl;
  return fwImpl->GetBundle(bundleId);
}

osgi::Bundle *BundleContextImpl::InstallBundle(
    std::string manifest, osgi::BundleActivator *activator) {
  // should never be null
  PLOG_INFO << "BundleContextImpl::InstallBundle";
  FrameworkImpl *fwImpl = this->frameworkImpl;
  return fwImpl->InstallBundle(manifest, activator);
}

std::vector<osgi::Bundle *> *BundleContextImpl::GetBundles() {
  PLOG_INFO << "BundleContextImpl::GetBundles";
  FrameworkImpl *fwImpl = this->frameworkImpl;
  return fwImpl->GetBundles();
}

}  // end namespace posgi
