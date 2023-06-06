#include "org/posgi/framework/impl/bundle_impl.h"

#include <plog/Log.h>

#include "org/osgi/framework/bundle.h"
#include "org/osgi/framework/bundle_activator.h"
#include "org/posgi/framework/impl/bundle_context_impl.h"

namespace posgi {

void BundleImpl::Start() {
  PLOG_INFO << "start " << this->to_string() << " (" << this->activatorRef
            << ")";
  this->SetState(BundleImpl::STARTING);
  // TODO(jhi): calling activator asynchronously?
  if (this->activatorRef != nullptr) {
    PLOG_INFO << "call activator start " << to_string() << " ("
              << this->activatorRef << ")";
    this->activatorRef->Start(this->bc);
  }
  this->SetState(BundleImpl::ACTIVE);
}
void BundleImpl::Stop() {
  PLOG_INFO << "stop " << to_string() << " (" << this->activatorRef << ")";
  this->SetState(BundleImpl::STOPPING);
  if (this->activatorRef != nullptr) {
    PLOG_INFO << "call activator stop " << to_string() << " ("
              << this->activatorRef << ")";
    this->activatorRef->Stop(this->bc);
  }
  this->SetState(BundleImpl::RESOLVED);
}

int BundleImpl::GetId() {
  return this->id;
}

std::string BundleImpl::GetSymbolicName() {
  return this->bundleSymbolicName;
}

osgi::BundleContext *BundleImpl::GetBundleContext() {
  return this->bc;
}

void BundleImpl::SetBundleContext(BundleContextImpl *bundleContextImpl) {
  PLOG_INFO << "set bundle context " << to_string();
  this->bc = bundleContextImpl;
}

int BundleImpl::GetState() {
  PLOG_INFO << to_string();
  return this->state;
}

void BundleImpl::SetState(int state) {
  PLOG_INFO << "set state " << this->getStateAsStringMapping(state) << " to "
            << to_string();
  this->state = state;
}

std::string BundleImpl::GetStateAsString() {
  std::string stateAsString = getStateAsStringMapping(this->state);
  PLOG_INFO << to_string();
  return stateAsString;
}

std::string BundleImpl::getStateAsStringMapping(int state) {
  switch (state) {
    case UNINSTALLED:
      return "UNINSTALLED";
    case INSTALLED:
      return "INSTALLED";
    case RESOLVED:
      return "RESOLVED";
    case STARTING:
      return "STARTING";
    case STOPPING:
      return "STOPPING";
    case ACTIVE:
      return "ACTIVE";
    default:
      return "UNKNOWN";
  }
}

// TODO(jhi): C++ pattern for toString() ?
std::string BundleImpl::to_string() {
  return "[" + std::to_string(this->id) + "] " + this->bundleSymbolicName +
         " [" + this->getStateAsStringMapping(this->state) + "]";
}

}  // end namespace posgi
