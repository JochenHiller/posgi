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
#include "org/posgi/framework/impl/utils/system_utils.h"

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

void usage() {
  std::cout << R"(Usage: posgi_cli [options]

Options:
  --version  show version number
  --help     show this help
    )" << std::endl;
}

RC do_main(std::vector<std::string> args) {
  if ((args.size() >= 2) &&
      (std::find(args.begin(), args.end(), "--help") != args.end())) {
    usage();
    return RC::kHelp;
  }
  if ((args.size() >= 2) &&
      (std::find(args.begin(), args.end(), "--version") != args.end())) {
    // report version
    std::cout << args[0] << " Version " << Posgi_VERSION_MAJOR << "."
              << Posgi_VERSION_MINOR << "-" << GIT_COMMIT_HASH << std::endl;
    std::cout << "Compiled with " << CMAKE_CXX_COMPILER_ID << " "
              << CMAKE_CXX_COMPILER_VERSION << std::endl;
    std::cout << "Running on OS " << Utils::SystemUtils::get_os() << std::endl;
    return RC::kVersion;
  }

  osgi::Framework *framework = osgi::FrameworkFactory().NewFramework();
  framework->Init();
  framework->Start();
  osgi::BundleContext *bc = framework->GetBundleContext();
  bc->InstallBundle(std::string(osgi::Constants::kBundleSymbolicName) +
                        ": BundleA_WithActivator",
                    new sample::SomeBundle("BundleA_WithActivator"));
  bc->InstallBundle(std::string(osgi::Constants::kBundleSymbolicName) +
                        ": BundleB_WithActivator",
                    new sample::SomeBundle("BundleB_WithActivator"));
  bc->InstallBundle(std::string(osgi::Constants::kBundleSymbolicName) +
                    ": BundleC_NoActivator");
  osgi::Bundle *consoleBundle = bc->InstallBundle(
      std::string(osgi::Constants::kBundleSymbolicName) + ": OsgiConsole",
      new osgi::OsgiConsole());
  consoleBundle->Start();

  // try out a dynamic cast, to show that we can refer to impl as well
  dynamic_cast<posgi::FrameworkImpl *>(framework)->dump_bundles();

  framework->WaitForStop(0);
  // framework->Stop();

  return RC::kOk;
}
