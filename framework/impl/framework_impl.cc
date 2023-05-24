#include "org/posgi/framework/impl/framework_impl.h"

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include <atomic>
#include <chrono>              // NOLINT [build/c++11]
#include <condition_variable>  // NOLINT [build/c++11]
#include <iostream>
#include <map>
#include <mutex>  // NOLINT [build/c++11]
#include <string>
#include <thread>  // NOLINT [build/c++11]
#include <vector>

#include "org/osgi/framework/constants.h"
#include "org/osgi/framework/launch/framework.h"
#include "org/posgi/framework/impl/bundle_context_impl.h"
#include "org/posgi/framework/impl/manifest_parser.h"

namespace posgi {

// TODO use inner namespace internal or impl?
// posgi::internal, posgi::impl

void initializeLogging() {
  static bool initLoggingDone = false;
  if (initLoggingDone) {
    return;
  }

  enum logging_type { file, console, color_console };
  logging_type type = logging_type::file;

  switch (type) {
    case file:
      // Option A) fixed file logger
      plog::init(plog::debug, "posgi.log");
      break;

    case console:
      // Option B) Console logging
      {
        static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::debug, &consoleAppender);
      }
      break;

    case color_console:
      // Option C) Color console logger
      {
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::debug, &consoleAppender);
      }
      break;

    default:
      break;
  }

  initLoggingDone = true;
}

FrameworkImpl::FrameworkImpl() {
  PLOG_INFO << "FrameworkImpl::FrameworkImpl";
}
FrameworkImpl::~FrameworkImpl() {
  PLOG_INFO << "FrameworkImpl::~FrameworkImpl";
}

void FrameworkImpl::dump_bundles() {
  PLOG_INFO << "FrameworkImpl::dump_bundles";
  PLOG_INFO << "FrameworkImpl::dump_bundles: this " << this;
  PLOG_INFO << "FrameworkImpl::dump_bundles: this->bundles " << this->bundles;
  for (const auto b : bundles) {
    PLOG_INFO << "  [" << b->id << "] " << b->bundleSymbolicName;
  }
}

void FrameworkImpl::frameworkThreadLoop() {
  PLOG_ERROR << "FrameworkImpl::frameworkThreadLoop started";

  {
    std::unique_lock<std::mutex> lock(framework_stop_mtx);
    while (!framework_stop) {
      framework_stop_cv.wait(lock);
    }
    PLOG_ERROR << "FrameworkImpl::frameworkThreadLoop: Signal received!";
  }

  {
    std::lock_guard<std::mutex> lockFinished(mtx);
    this->isThreadFinished = true;
  }
  PLOG_ERROR << "FrameworkImpl::frameworkThreadLoop: before notify!";
  this->cv.notify_one();
  PLOG_ERROR << "FrameworkImpl::frameworkThreadLoop: after notify!";
}

void FrameworkImpl::Init() {
  PLOG_INFO << "FrameworkImpl::Init";
  static bool initDone = false;
  if (initDone) {
    return;
  }
  initializeLogging();

  this->id = 0;
  this->bundleSymbolicName = "org.posgi.framework.impl.FrameworkImpl";
  BundleContextImpl *bc = new BundleContextImpl(this);
  bc->dump_bundle_context();
  this->SetBundleContext(bc);
  this->bundles = std::vector<BundleImpl *>();
  this->bundles.push_back(this);

  initDone = true;
}

void FrameworkImpl::Start() {
  PLOG_INFO << "FrameworkImpl::Start";
  BundleImpl::Start();
  // TODO: give thread a good name, see named thread sample
  frameworkThread = new std::thread(&FrameworkImpl::frameworkThreadLoop, this);
  // give thread a chance to start
  std::this_thread::yield();
}

void FrameworkImpl::Stop() {
  PLOG_INFO << "FrameworkImpl::Stop";
  BundleImpl::Stop();

  // run in its own scope to free mutex
  {
    std::lock_guard<std::mutex> lock(framework_stop_mtx);
    framework_stop = true;

    PLOG_ERROR << "FrameworkImpl::Stop (notify thread)";
    framework_stop_cv.notify_one();
    PLOG_ERROR << "FrameworkImpl::Stop (notify thread done)";
    try {
      // TODO: why do we get a "thread::join failed: Invalid argument" here?
      frameworkThread->join();
    } catch (const std::system_error &err) {
      PLOG_ERROR << "FrameworkImpl::Stop: caught exception: "
                 << err.what();  // << err;
    }
  }

  PLOG_INFO << "FrameworkImpl::Stop (done)";
}

void FrameworkImpl::dump_headers(std::map<std::string, std::string> headers) {
  PLOG_INFO << "FrameworkImpl::dump_headers()";
  for (const auto &[key, value] : headers) {
    PLOG_INFO << key << ": " << value;
  }
}
osgi::Bundle *FrameworkImpl::GetBundle(int bundleId) {
  PLOG_INFO << "FrameworkImpl::GetBundle (" << bundleId << ")";
  dump_bundles();
  PLOG_ERROR << "FrameworkImpl::GetBundle (" << this << ")";
  PLOG_ERROR << "FrameworkImpl::GetBundle (" << this->bundles << ")";
  if (bundleId < 0 || bundleId >= this->bundles.size()) {
    PLOG_ERROR << "FrameworkImpl::GetBundle: Invalid bundle id: " << bundleId;
    return nullptr;
  }
  auto bundle = this->bundles.at(bundleId);
  PLOG_INFO << "FrameworkImpl::GetBundle: " << bundle;
  PLOG_INFO << "FrameworkImpl::GetBundle (bsn: " << bundle->bundleSymbolicName
            << ")";
  return bundle;
}

std::vector<osgi::Bundle *> *FrameworkImpl::GetBundles() {
  PLOG_INFO << "FrameworkImpl::GetBundles";
  dump_bundles();
  std::vector<osgi::Bundle *> *allBundles = new std::vector<osgi::Bundle *>();
  for (const auto b : bundles) {
    osgi::Bundle *b2 = dynamic_cast<osgi::Bundle *>(b);
    allBundles->push_back(b2);
  }
  return allBundles;
}

osgi::Bundle *FrameworkImpl::InstallBundle(std::string manifest,
                                           osgi::BundleActivator *activator) {
  PLOG_INFO << "FrameworkImpl::InstallBundle";

  ManifestParser *parser = new ManifestParser();
  std::map<std::string, std::string> headers = parser->parse(manifest);
  dump_headers(headers);
  delete parser;

  BundleImpl *bundleImpl = new BundleImpl();
  // inc bundle id
  bundleImpl->id = ++lastBundleId;
  bundleImpl->bundleSymbolicName =
      headers[osgi::Constants::BUNDLE_SYMBOLICNAME];
  if (activator != nullptr) {
    bundleImpl->activatorRef = activator;
  }
  bundleImpl->SetState(BundleImpl::INSTALLED);
  PLOG_INFO << "FrameworkImpl::InstallBundle " << bundleImpl;
  BundleContextImpl *bundleContextImpl = new BundleContextImpl(bundleImpl);
  bundleContextImpl->frameworkImpl = this;
  bundleImpl->SetBundleContext(bundleContextImpl);
  bundles.push_back(bundleImpl);
  // TODO: any other state than resolved?
  bundleImpl->SetState(BundleImpl::RESOLVED);

  return bundleImpl;
}

// Wait until framework is stopped or timeout occurs.
// Returns 0 if framework is stopped, 1 if timeout occurs.
int FrameworkImpl::WaitForStop(long timeout) {
  PLOG_INFO << "FrameworkImpl::WaitForStop (" << timeout << " ms)";

  if (timeout == 0) {
    // https://stackoverflow.com/questions/66773247/libcabi-dylib-terminating-with-uncaught-exception-of-type-std-1system-er
    try {
      PLOG_ERROR << "FrameworkImpl::WaitForStop (framework_stop_mtx1"
                 << ")";
      frameworkThread->join();
      PLOG_ERROR << "FrameworkImpl::WaitForStop (framework_stop_mtx2"
                 << ")";

    } catch (const std::system_error &err) {
      PLOG_ERROR << "FrameworkImpl::WaitForStop (framework_stop_mtx3"
                 << ")";

      PLOG_ERROR
          << "FrameworkImpl::WaitForStop: caught exception: ";  // << err;
    }
    PLOG_INFO << "FrameworkImpl::WaitForStop: Thread finished successfully.";
    return 0;
  }

  std::unique_lock<std::mutex> lockFinished(mtx);
  // https://stackoverflow.com/questions/38595834/compilation-error-this-cannot-be-implicitly-captured-in-this-context
  if (cv.wait_for(lockFinished, std::chrono::milliseconds(timeout),
                  [this] { return isThreadFinished; })) {
    PLOG_INFO << "FrameworkImpl::WaitForStop: Thread finished successfully.";
    return 0;
  } else {
    PLOG_INFO << "FrameworkImpl::WaitForStop: Timeout occurred while waiting "
                 "for the thread.";
    return 1;
  }
}

}  // end namespace posgi
