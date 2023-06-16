#ifndef POSGI_ORG_OSGI_SERVICE_CONSOLE_IMPL_OSGI_CONSOLE_H
#define POSGI_ORG_OSGI_SERVICE_CONSOLE_IMPL_OSGI_CONSOLE_H

#include <string>

#include "org/osgi/framework/bundle_activator.h"
#include "org/osgi/framework/bundle_context.h"
#include "org/osgi/service/console/console.h"

namespace osgi {

class OsgiConsole : public Console, public BundleActivator {
  void Start(osgi::BundleContext *bundleContext);
  void Stop(osgi::BundleContext *bundleContext);

  std::vector<osgi::Command *> getCommands();

 private:
  void console();
  Command *findCommand(const std::string &scope_function_to_search);
  void show_help();
  void show_help_for_command(std::vector<std::string> args);

  osgi::Bundle *getBundleFromSecondArg(
      const std::vector<std::string> &command_args);

  osgi::BundleContext *bundleContext_;
  std::thread *consoleThread_;
  std::vector<Command *> commands_ = {};
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_SERVICE_CONSOLE_IMPL_OSGI_CONSOLE_H
