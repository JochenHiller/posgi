#include "org/posgi/framework/impl/bundle_impl.h"

#include <plog/Log.h>

#include "org/osgi/framework/bundle.h"
#include "org/osgi/framework/bundle_activator.h"
#include "org/posgi/framework/impl/bundle_context_impl.h"

namespace posgi {

void BundleImpl::Start() {
  PLOG_INFO << "BundleImpl::Start (" << this->id << ", "
            << this->bundleSymbolicName << ", " << this->activatorRef << ")";
  if (this->activatorRef != nullptr) {
    PLOG_INFO << "BundleImpl::Start::Activator (" << this->id << ", "
              << this->bundleSymbolicName << ", " << this->activatorRef << ")";
    this->activatorRef->Start(this->bc);
  }
}
void BundleImpl::Stop() {
  PLOG_INFO << "BundleImpl::Stop (" << this->id << ", "
            << this->bundleSymbolicName << ")";
  if (this->activatorRef != nullptr) {
    PLOG_INFO << "BundleImpl::Start::Activator (" << this->id << ", "
              << this->bundleSymbolicName << ")";
    this->activatorRef->Stop(this->bc);
  }
}
int BundleImpl::GetId() {
  // PLOG_INFO << "BundleImpl::GetId";
  return this->id;
}
std::string BundleImpl::GetSymbolicName() {
  // PLOG_INFO << "BundleImpl::GetSymbolicName";
  return this->bundleSymbolicName;
}
osgi::BundleContext *BundleImpl::GetBundleContext() {
  // PLOG_INFO << "BundleImpl::GetBundleContext";
  return bc;
};
void BundleImpl::SetBundleContext(BundleContextImpl *bundleContextImpl) {
  PLOG_INFO << "BundleImpl::SetBundleContext";
  this->bc = bundleContextImpl;
};

} // end namespace posgi