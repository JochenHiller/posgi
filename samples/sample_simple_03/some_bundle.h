#ifndef POSGI_SAMPLES_SAMPLE_SIMPLE_02_SOME_BUNDLE_H
#define POSGI_SAMPLES_SAMPLE_SIMPLE_02_SOME_BUNDLE_H

#include <org/osgi/framework/bundle_activator.h>
#include <org/osgi/framework/bundle_context.h>

#include <iostream>
#include <string>

namespace sample {

class SomeBundle : public osgi::BundleActivator {
 public:
  SomeBundle(std::string name);

  void Start(osgi::BundleContext *bundleContext);
  void Stop(osgi::BundleContext *bundleContext);

  std::string MANIFEST();

 private:
  std::string name_;
};

}  // namespace sample

#endif  // POSGI_SAMPLES_SAMPLE_SIMPLE_02_SOME_BUNDLE_H
