#ifndef POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_H
#define POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_H

#include <map>
#include <string>

#include "org/osgi/framework/bundle_context.h"
#include "org/osgi/framework/bundle_revision.h"

namespace osgi {

class Bundle : public virtual BundleRevision {
public:
  // TODO enums?
  static const int UNINSTALLED = 0x00000001;
  static const int INSTALLED = 0x00000002;
  static const int RESOLVED = 0x00000004;
  static const int STARTING = 0x00000008;
  static const int STOPPING = 0x00000010;
  static const int ACTIVE = 0x00000020;

  // start/stop options
  static const int START_TRANSIENT = 0x00000001;
  static const int START_ACTIVATION_POLICY = 0x00000002;
  static const int STOP_TRANSIENT = 0x00000001;

  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual BundleContext *GetBundleContext() = 0;
  virtual int GetId() = 0;
  virtual std::string GetSymbolicName() = 0;
  virtual int GetState() = 0;
  // TODO: convenient method to get the state as a string, move to utils?
  virtual std::string GetStateAsString() = 0;
};

} // end namespace osgi

#endif // POSGI_ORG_OSGI_FRAMEWORK_BUNDLE_H
