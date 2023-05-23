#ifndef POSGI_ORG_OSGI_FRAMEWORK_LAUNCH_FRAMEWORK_FACTORY_H
#define POSGI_ORG_OSGI_FRAMEWORK_LAUNCH_FRAMEWORK_FACTORY_H

#include "org/osgi/framework/launch/framework.h"

namespace osgi {

class FrameworkFactory {
public:
  FrameworkFactory();
  static Framework *NewFramework();
};

} // end namespace osgi

#endif // POSGI_ORG_OSGI_FRAMEWORK_LAUNCH_FRAMEWORK_FACTORY_H
