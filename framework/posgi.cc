#include "posgi.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "PosgiConfig.h"
#include "org/osgi/framework/bundle_activator.h"
#include "org/osgi/framework/constants.h"
#include "org/osgi/framework/launch/framework.h"
#include "org/osgi/framework/launch/framework_factory.h"
#include "org/osgi/framework/service/osgi_console.h"
#include "org/posgi/framework/impl/framework_impl.h"

namespace sample {

class SomeBundle : public osgi::BundleActivator {
 public:
  // use a member constructor initializer list
  explicit SomeBundle(std::string name) : name(name) {
    std::cout << "SomeBundle::SomeBundle(" << name << ")\n";
  }
  virtual ~SomeBundle() = default;

  void Start(osgi::BundleContext *bundleContext) {
    std::cout << name << "::Start\n";
  }
  void Stop(osgi::BundleContext *bundleContext) {
    std::cout << name << "::Stop\n";
  }

 private:
  std::string name;
};

}  // namespace sample

int do_main(std::vector<std::string> args) {
  if ((args.size() >= 2) &&
      (std::find(args.begin(), args.end(), "--version") != args.end())) {
    // report version
    std::cout << args[0] << " Version " << Posgi_VERSION_MAJOR << "."
              << Posgi_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << args[0] << " [--version]" << std::endl;
    return RC_VERSION;
  }

  osgi::Framework *framework = osgi::FrameworkFactory().NewFramework();
  framework->Init();
  framework->Start();
  osgi::BundleContext *bc = framework->GetBundleContext();
  osgi::Bundle *bundleA =
      bc->InstallBundle(osgi::Constants::BUNDLE_SYMBOLICNAME + ": BundleA",
                        new sample::SomeBundle("BundleA"));
  osgi::Bundle *bundleB =
      bc->InstallBundle(osgi::Constants::BUNDLE_SYMBOLICNAME + ": BundleB",
                        new sample::SomeBundle("BundleB"));
  osgi::Bundle *bundleC =
      bc->InstallBundle(osgi::Constants::BUNDLE_SYMBOLICNAME + ": BundleC");
  bundleA->Start();
  bundleB->Start();
  bundleC->Start();

  osgi::Bundle *consoleBundle =
      bc->InstallBundle(osgi::Constants::BUNDLE_SYMBOLICNAME + ": OsgiConsole",
                        new osgi::OsgiConsole());
  consoleBundle->Start();

  // try out a dynamic cast, to show that we can refer to impl as well
  dynamic_cast<posgi::FrameworkImpl *>(framework)->dump_bundles();
  bundleC->Stop();
  bundleB->Stop();
  bundleA->Stop();

  framework->WaitForStop(0);
  // framework->Stop();

  return RC_OK;
}
