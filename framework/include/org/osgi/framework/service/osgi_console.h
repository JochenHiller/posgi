#ifndef POSGI_ORG_OSGI_FRAMEWORK_SERVICE_OSGI_CONSOLE_H
#define POSGI_ORG_OSGI_FRAMEWORK_SERVICE_OSGI_CONSOLE_H

#include <thread>

#include "org/osgi/framework/bundle_activator.h"

namespace osgi {

class OsgiConsole : public osgi::BundleActivator {
 public:
  OsgiConsole();
  void Start(osgi::BundleContext *bundleContext);
  void Stop(osgi::BundleContext *bundleContext);

 private:
  void console();
  osgi::Bundle *getBundleFromSecondArg(
      const std::vector<std::string> &command_args);

  osgi::BundleContext *bundleContext;
  std::thread *consoleThread;
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_FRAMEWORK_SERVICE_OSGI_CONSOLE_H
