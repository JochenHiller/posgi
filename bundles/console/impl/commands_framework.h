#ifndef POSGI_ORG_OSGI_SERVICE_CONSOLE_IMPL_COMMANDS_FRAMEWORK_H
#define POSGI_ORG_OSGI_SERVICE_CONSOLE_IMPL_COMMANDS_FRAMEWORK_H

#include <string>
#include <vector>

#include "org/osgi/framework/bundle_context.h"
#include "org/osgi/service/console/command.h"

namespace osgi {

class CommandStartBundle : public Command {
 public:
  CommandStartBundle(BundleContext *bc);
  std::string scope();
  std::string function();
  std::string help_short();
  std::string help();
  void execute(std::vector<std::string> args);

 private:
  BundleContext *bundleContext_;
};

class CommandStopBundle : public Command {
 public:
  CommandStopBundle(BundleContext *bc);
  std::string scope();
  std::string function();
  std::string help_short();
  std::string help();
  void execute(std::vector<std::string> args);

 private:
  BundleContext *bundleContext_;
};

class CommandExitFramework : public Command {
 public:
  CommandExitFramework(BundleContext *bc);
  std::string scope();
  std::string function();
  std::string help_short();
  std::string help();
  void execute(std::vector<std::string> args);

 private:
  BundleContext *bundleContext_;
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_SERVICE_CONSOLE_IMPL_COMMANDS_FRAMEWORK_H
