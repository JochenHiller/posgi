#include "org/osgi/framework/launch/framework_factory.h"

#include <plog/Appenders/IAppender.h>
#include <plog/Init.h>
#include <plog/Log.h>

#include <iostream>

#include "org/osgi/framework/launch/framework.h"
#include "org/posgi/framework/impl/framework_impl.h"

namespace osgi {

FrameworkFactory::FrameworkFactory() {
  PLOG_INFO << "FrameworkFactory::FrameworkFactory()";
}

Framework *FrameworkFactory::NewFramework() {
  PLOG_INFO << "FrameworkFactory::NewFramework()";
  // Bundle *bundle = new posgi::BundleImpl();
  Framework *framework = new posgi::FrameworkImpl();
  return framework;
  // return nullptr;
}

} // end namespace osgi
