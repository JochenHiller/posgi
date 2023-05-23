/*
 * Simple program to to install and start & stop one single bundle.
 */

#include <iostream>

#include "org/osgi/framework/bundle_context.h"
#include "org/osgi/framework/constants.h"
#include "org/osgi/framework/launch/framework.h"
#include "org/osgi/framework/launch/framework_factory.h"
#include "some_bundle.h"

int main() {
  std::cout << "Starting OSGi framework\n";
  osgi::Framework *framework = osgi::FrameworkFactory().NewFramework();
  framework->Init();
  framework->Start();
  osgi::BundleContext *bundleContext = framework->GetBundleContext();

  std::cout << "Installing bundle A\n";
  osgi::Bundle *bundleA = bundleContext->InstallBundle(
      osgi::Constants::BUNDLE_SYMBOLICNAME + ": BundleA",
      new sample::SomeBundle("BundleA"));

  bundleA->Start();
  bundleA->Stop();

  std::cout << "Stopping OSGi framework\n";
  framework->Stop();
  return 0;
}
