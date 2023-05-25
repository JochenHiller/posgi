#include "posgi.h"  // NOLINT(build/include_subdir)

#include <iostream>
#include <string>

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

// https://github.com/llvm/llvm-project/issues/54668 ==> ignore
// bugprone-exception-escape

int do_main(int argc, char *argv[]) {
  if ((argc >= 2) && (strcmp(argv[1], "--version") == 0)) {
    // report version
    std::cout << argv[0] << " Version " << Posgi_VERSION_MAJOR << "."
              << Posgi_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " [--version]" << std::endl;
    return 1;
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

  return 0;
}