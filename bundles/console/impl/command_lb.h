#ifndef POSGI_ORG_OSGI_SERVICE_CONSOLE_IMPL_COMMAND_LB_H
#define POSGI_ORG_OSGI_SERVICE_CONSOLE_IMPL_COMMAND_LB_H

#include <string>

#include "org/osgi/framework/bundle_context.h"
#include "org/osgi/service/console/command.h"

namespace osgi {

class CommandListBundles : public Command {
 public:
  CommandListBundles(BundleContext *bc);
  std::string scope();
  std::string function();
  std::string help_short();
  std::string help();
  void execute(std::vector<std::string> args);

 private:
  BundleContext *bundleContext_;
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_SERVICE_CONSOLE_IMPL_COMMAND_LB_H
