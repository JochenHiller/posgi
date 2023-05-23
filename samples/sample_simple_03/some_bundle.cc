#include "some_bundle.h"

#include <iostream>
#include <string>

#include "org/osgi/framework/bundle_context.h"

namespace sample {

SomeBundle::SomeBundle(std::string name) {
  std::cout << "SomeBundle::SomeBundle(" << name << ")\n";
  this->name = name;
}
void SomeBundle::Start(osgi::BundleContext *bundleContext) {
  std::cout << name << "::Start\n";
}
void SomeBundle::Stop(osgi::BundleContext *bundleContext) {
  std::cout << name << "::Stop\n";
}

std::string SomeBundle::MANIFEST() {
  return "Bundle-SymbolicName: " + name +
         "\n"
         "Bundle-Version: 1.0.0\n"
         "Bundle-Activator: " +
         name + "\n";
}

} // namespace sample
