/**
 * @brief Sample to test psogi design in C++.
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <boost/algorithm/string.hpp>

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

namespace osgi {

class BundleRevision {
public:
  virtual std::string GetSymbolicName() = 0;
};

class Bundle;
class BundleActivator;

class BundleContext {
public:
  virtual Bundle *InstallBundle(std::string manifest,
                                BundleActivator *activator = nullptr) = 0;
  virtual Bundle *GetBundle(int bundleId) = 0;
  virtual std::vector<Bundle *> *GetBundles() = 0;
};

class BundleActivator {
public:
  virtual void Start(BundleContext *bundleContext) = 0;
  virtual void Stop(BundleContext *bundleContext) = 0;
};

class Bundle : public virtual BundleRevision {
public:
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual BundleContext *GetBundleContext() = 0;
  virtual int GetId() = 0;
  virtual std::string GetSymbolicName() = 0;
};

class Framework : public virtual Bundle {
public:
  virtual void Init() = 0;
  virtual int WaitForStop(long timeout) = 0;
};

class Constants {
public:
  // inline supported since C++17:
  // https://en.cppreference.com/w/cpp/language/inline
  inline static const std::string BUNDLE_ACTIVATOR = "Bundle-Activator";
  inline static const std::string BUNDLE_SYMBOLICNAME = "Bundle-SymbolicName";
  inline static const std::string BUNDLE_VERSION = "Bundle-Version";
};

} // namespace osgi

namespace posgi {

class BundleImpl;
class FrameworkImpl;

class BundleContextImpl : public osgi::BundleContext {
public:
  BundleContextImpl(BundleImpl *bundleImpl);
  BundleContextImpl(FrameworkImpl *frameworkImpl);
  osgi::Bundle *GetBundle(int bundleId);
  std::vector<osgi::Bundle *> *GetBundles();

  osgi::Bundle *InstallBundle(std::string manifest,
                              osgi::BundleActivator *activator = nullptr);

  void dump_bundle_context();
  // TODO: make private and provide a setter
  FrameworkImpl *frameworkImpl;

private:
  BundleImpl *bundleImpl;
};

class BundleImpl : public virtual osgi::Bundle {
public:
  void Start() {
    PLOG_INFO << "BundleImpl::Start (" << this->id << ", "
              << this->bundleSymbolicName << ", " << this->activatorRef << ")";
    if (this->activatorRef != nullptr) {
      PLOG_INFO << "BundleImpl::Start::Activator (" << this->id << ", "
                << this->bundleSymbolicName << ", " << this->activatorRef
                << ")";
      this->activatorRef->Start(this->bc);
    }
  }
  void Stop() {
    PLOG_INFO << "BundleImpl::Stop (" << this->id << ", "
              << this->bundleSymbolicName << ")";
    if (this->activatorRef != nullptr) {
      PLOG_INFO << "BundleImpl::Start::Activator (" << this->id << ", "
                << this->bundleSymbolicName << ")";
      this->activatorRef->Stop(this->bc);
    }
  }
  int GetId() {
    // PLOG_INFO << "BundleImpl::GetId";
    return this->id;
  }
  std::string GetSymbolicName() {
    // PLOG_INFO << "BundleImpl::GetSymbolicName";
    return this->bundleSymbolicName;
  }
  osgi::BundleContext *GetBundleContext() {
    // PLOG_INFO << "BundleImpl::GetBundleContext";
    return bc;
  };
  void SetBundleContext(BundleContextImpl *bundleContextImpl) {
    PLOG_INFO << "BundleImpl::SetBundleContext";
    this->bc = bundleContextImpl;
  };

public:
  // TODO: make all private with setters, or use friends
  BundleContextImpl *bc;
  int id;
  std::string bundleSymbolicName;
  osgi::BundleActivator *activatorRef = nullptr;
};

class ManifestParser {
public:
  ManifestParser() { PLOG_INFO << "ManifestParser::ManifestParser()"; }
  std::map<std::string, std::string> parse(std::string manifest) {
    PLOG_INFO << "ManifestParser::parse()";
    std::map<std::string, std::string> headers;
    // read from string as input string stream
    std::istringstream iss(manifest);
    std::string line;
    while (std::getline(iss, line)) {
      // Split the line by the delimiter ":"
      std::vector<std::string> tokens;
      boost::split(tokens, line, boost::is_any_of(":"));
      // TODO: what to do with "key: value1:value2"
      if (tokens.size() != 2) {
        PLOG_ERROR << "Could not parse '" << line << "'";
        continue;
      } else {
        std::string key = tokens.at(0);
        boost::trim(key);
        std::string value = tokens.at(1);
        boost::trim(value);
        // Insert the key-value pair into the map
        headers[key] = value;
      }
    }
    return headers;
  }
};

class FrameworkImpl : public BundleImpl, public osgi::Framework {
public:
  FrameworkImpl() { PLOG_INFO << "FrameworkImpl::FrameworkImpl"; }
  ~FrameworkImpl() { PLOG_INFO << "FrameworkImpl::~FrameworkImpl"; }

  void frameworkThreadLoop() {
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
    this->cv.notify_one();
  }

  void Init() {
    PLOG_INFO << "FrameworkImpl::Init";
    this->id = 0;
    this->bundleSymbolicName = "org.posgi.framework.impl.FrameworkImpl";
    BundleContextImpl *bc = new BundleContextImpl(this);
    bc->dump_bundle_context();
    this->SetBundleContext(bc);
    this->bundles = std::vector<BundleImpl *>();
    this->bundles.push_back(this);
  }

  void Start() {
    PLOG_INFO << "FrameworkImpl::Start";
    BundleImpl::Start();
    // TODO: give thread a good name, see named thread sample
    frameworkThread =
        new std::thread(&FrameworkImpl::frameworkThreadLoop, this);
    // give thread a chance to start
    std::this_thread::yield();
  }

  void Stop() {
    PLOG_INFO << "FrameworkImpl::Stop";
    BundleImpl::Stop();

    // run in its own scope to free mutex
    {
      std::lock_guard<std::mutex> lock(framework_stop_mtx);
      framework_stop = true;
    }
    PLOG_ERROR << "FrameworkImpl::Stop (notify thread)";
    framework_stop_cv.notify_one();
    PLOG_ERROR << "FrameworkImpl::Stop (notify thread done)";
    frameworkThread->join();
    PLOG_INFO << "FrameworkImpl::Stop (done)";
  }

  void dump_headers(std::map<std::string, std::string> headers) {
    PLOG_INFO << "FrameworkImpl::dump_headers()";
    for (const auto &[key, value] : headers) {
      PLOG_INFO << key << ": " << value;
    }
  }
  osgi::Bundle *GetBundle(int bundleId) {
    PLOG_INFO << "FrameworkImpl::GetBundle (" << bundleId << ")";
    dump_bundles();
    PLOG_ERROR << "FrameworkImpl::GetBundle (" << this << ")";
    PLOG_ERROR << "FrameworkImpl::GetBundle (" << this->bundles << ")";
    auto bundle = this->bundles.at(bundleId);
    PLOG_INFO << "FrameworkImpl::GetBundle: " << bundle;
    PLOG_INFO << "FrameworkImpl::GetBundle (bsn: " << bundle->bundleSymbolicName
              << ")";
    return bundle;
  }

  std::vector<osgi::Bundle *> *GetBundles() {
    PLOG_INFO << "FrameworkImpl::GetBundles";
    dump_bundles();
    std::vector<osgi::Bundle *> *allBundles = new std::vector<osgi::Bundle *>();
    for (const auto b : bundles) {
      osgi::Bundle *b2 = dynamic_cast<osgi::Bundle *>(b);
      allBundles->push_back(b2);
    }
    return allBundles;
  }

  osgi::Bundle *InstallBundle(std::string manifest,
                              osgi::BundleActivator *activator = nullptr) {
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
    PLOG_INFO << "FrameworkImpl::InstallBundle " << bundleImpl;
    BundleContextImpl *bundleContextImpl = new BundleContextImpl(bundleImpl);
    bundleContextImpl->frameworkImpl = this;
    bundleImpl->SetBundleContext(bundleContextImpl);
    bundles.push_back(bundleImpl);
    return bundleImpl;
  }
  void dump_bundles() {
    PLOG_INFO << "FrameworkImpl::dump_bundles";
    PLOG_INFO << "FrameworkImpl::dump_bundles: this " << this;
    PLOG_INFO << "FrameworkImpl::dump_bundles: this->bundles " << this->bundles;
    for (const auto b : bundles) {
      PLOG_INFO << "  [" << b->id << "] " << b->bundleSymbolicName;
    }
  }

  // Wait until framework is stopped or timeout occurs.
  // Returns 0 if framework is stopped, 1 if timeout occurs.
  int WaitForStop(long timeout) {
    PLOG_INFO << "FrameworkImpl::WaitForStop (" << timeout << " ms)";

    if (timeout == 0) {
      frameworkThread->join();
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

private:
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

void BundleContextImpl::dump_bundle_context() {
  PLOG_INFO << "BundleContextImpl::dump_bundle_context";
  PLOG_ERROR << "BundleContextImpl::dump_bundle_context " << this;
  PLOG_ERROR << "BundleContextImpl::dump_bundle_context "
             << this->frameworkImpl;
  PLOG_ERROR << "BundleContextImpl::dump_bundle_context " << this->bundleImpl;
}
BundleContextImpl::BundleContextImpl(BundleImpl *bundleImpl) {
  PLOG_INFO << "BundleContextImpl::BundleContextImpl(BundleImpl)";
  this->bundleImpl = bundleImpl;
}
BundleContextImpl::BundleContextImpl(FrameworkImpl *frameworkImpl) {
  PLOG_INFO << "BundleContextImpl::BundleContextImpl(FrameworkImpl)";
  this->frameworkImpl = frameworkImpl;
  // preserve as well as bundle context
  this->bundleImpl = frameworkImpl;
}
osgi::Bundle *BundleContextImpl::GetBundle(int bundleId) {
  PLOG_INFO << "BundleContextImpl::GetBundle";
  FrameworkImpl *fwImpl = this->frameworkImpl;
  return fwImpl->GetBundle(bundleId);
}

osgi::Bundle *
BundleContextImpl::InstallBundle(std::string manifest,
                                 osgi::BundleActivator *activator) {
  // should never be null
  PLOG_INFO << "BundleContextImpl::InstallBundle";
  FrameworkImpl *fwImpl = this->frameworkImpl;
  return fwImpl->InstallBundle(manifest, activator);
}

std::vector<osgi::Bundle *> *BundleContextImpl::GetBundles() {
  PLOG_INFO << "BundleContextImpl::GetBundles";
  FrameworkImpl *fwImpl = this->frameworkImpl;
  return fwImpl->GetBundles();
}

class OsgiConsole : public osgi::BundleActivator {
public:
  OsgiConsole() { PLOG_INFO << "OsgiConsole::OsgiConsole"; }
  void Start(osgi::BundleContext *bundleContext) {
    PLOG_INFO << "OsgiConsole::Start";
    this->bundleContext = bundleContext;
    consoleThread = new std::thread(&OsgiConsole::console, this);
  }
  void Stop(osgi::BundleContext *bundleContext) {
    PLOG_INFO << "OsgiConsole::Stop";
    this->bundleContext = nullptr;
  }

  void console() {
    while (true) {
      std::string command;
      // Prompt for command
      std::cout << "osgi> ";

      // Read command from stdin
      std::getline(std::cin, command);
      PLOG_INFO << "OsgiConsole::console: " << command;

      // Check the command and perform corresponding actions
      if (command == "help") {
        std::cout << "Available commands" << std::endl;
        std::cout << "  help  Show this help" << std::endl;
        std::cout << "  lb    List bundles" << std::endl;
        std::cout << "  quit  Quit OSGi console" << std::endl;
        std::cout << "  exit  Exit OSGi framework, measns stop framework"
                  << std::endl;
      } else if (command == "quit") {
        std::cout << "Quiting the OSGi console" << std::endl;
        break;
      } else if (command == "exit") {
        std::cout << "Exit OSGi framework" << std::endl;
        this->bundleContext->GetBundle(0)->Stop();
        std::cout << "Exit OSGi framework after" << std::endl;
      } else if (command == "lb") {
        std::cout << "List bundles" << std::endl;
        PLOG_ERROR << "OsgiConsole::console: " << this;
        PLOG_ERROR << "OsgiConsole::console: " << this->bundleContext;

        auto bundles = this->bundleContext->GetBundles();
        for (const auto b : *bundles) {
          std::cout << "  [" << b->GetId() << "] " << b->GetSymbolicName()
                    << std::endl;
        }
      } else {
        // std::cout << "Unknown command: " << command << std::endl;
      }
    }
  }

private:
  osgi::BundleContext *bundleContext;
  std::thread *consoleThread;
};

} // namespace posgi

namespace sample {

class SomeBundle : public osgi::BundleActivator {
public:
  SomeBundle(std::string name) {
    PLOG_INFO << "SomeBundle::SomeBundle(" << name << ")";
    this->name = name;
  }
  void Start(osgi::BundleContext *bundleContext) {
    PLOG_INFO << name << "::Start";
  }
  void Stop(osgi::BundleContext *bundleContext) {
    PLOG_INFO << name << "::Stop";
  }

private:
  std::string name;
};

} // namespace sample

int main() {
  // console logging
  // static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  // plog::init(plog::debug, &consoleAppender);
  // file logging
  plog::init(plog::debug, "posgi.log");

  PLOG_INFO << "main";
  osgi::Framework *fw = new posgi::FrameworkImpl();
  fw->Init();
  fw->Start();
  osgi::BundleContext *bc = fw->GetBundleContext();
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
                        new posgi::OsgiConsole());
  consoleBundle->Start();

  // try out a dynamic cast, to show that we can refer to impl as well
  dynamic_cast<posgi::FrameworkImpl *>(fw)->dump_bundles();
  bundleC->Stop();
  bundleB->Stop();
  bundleA->Stop();

  fw->WaitForStop(0);
  // fw->Stop();
  PLOG_INFO << "main ended";

  return 0;
}
