#include "some_bundle.h"  // NOLINT [build/include_subdir]

#include <org/osgi/framework/bundle_context.h>

#include <iostream>
#include <string>

namespace sample {

SomeBundle::SomeBundle(std::string name) : name_(name) {
  std::cout << "SomeBundle::SomeBundle(" << name_ << ")\n";
}
void SomeBundle::Start(osgi::BundleContext *bundleContext) {
  std::cout << name_ << "::Start\n";
}
void SomeBundle::Stop(osgi::BundleContext *bundleContext) {
  std::cout << name_ << "::Stop\n";
}

std::string SomeBundle::MANIFEST() {
  return "Bundle-SymbolicName: " + name_ +
         "\n"
         "Bundle-Version: 1.0.0\n"
         "Bundle-Activator: " +
         name_ + "\n";
}

}  // namespace sample
