#ifndef POSGI_ORG_POSGI_FRAMEWORK_IMPL_FRAMEWORK_IMPL_H
#define POSGI_ORG_POSGI_FRAMEWORK_IMPL_FRAMEWORK_IMPL_H

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "org/osgi/framework/bundle_context.h"
#include "org/osgi/framework/launch/framework.h"
#include "org/posgi/framework/impl/bundle_impl.h"

namespace posgi {

// forward declaration needed to dependency of FrameworkImpl <-> BundleImpl
// class BundleImpl;

class FrameworkImpl : public BundleImpl, public osgi::Framework {
 public:
  FrameworkImpl();
  ~FrameworkImpl();

  void Init();
  void Start();
  void Stop();

  void dump_headers(std::map<std::string, std::string> headers);
  osgi::Bundle *GetBundle(int bundleId);
  std::vector<osgi::Bundle *> *GetBundles();
  osgi::Bundle *InstallBundle(std::string manifest,
                              osgi::BundleActivator *activator = nullptr);
  void dump_bundles();

  // Wait until framework is stopped or timeout occurs.
  // Returns 0 if framework is stopped, 1 if timeout occurs.
  int WaitForStop(int timeout);

 private:
  void frameworkThreadLoop();

  std::vector<BundleImpl *> bundles;
  int lastBundleId = 0;
  std::thread *frameworkThread;

  std::condition_variable cv;
  std::mutex mtx;
  bool isThreadFinished = false;

  std::mutex framework_stop_mtx;
  std::condition_variable framework_stop_cv;
  bool framework_stop = false;
};

}  // end namespace posgi

#endif  // POSGI_ORG_POSGI_FRAMEWORK_IMPL_FRAMEWORK_IMPL_H
