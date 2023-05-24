#ifndef POSGI_ORG_OSGI_FRAMEWORK_LAUNCH_FRAMEWORK_H
#define POSGI_ORG_OSGI_FRAMEWORK_LAUNCH_FRAMEWORK_H

#include "org/osgi/framework/bundle.h"

namespace osgi {

class Framework : public virtual Bundle {
 public:
  virtual void Init() = 0;
  virtual int WaitForStop(int timeout) = 0;
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_FRAMEWORK_LAUNCH_FRAMEWORK_H
