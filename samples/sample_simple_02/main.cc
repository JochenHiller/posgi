/**
 * Sample to test class hierarchy design in C++.
 */
#include <iostream>
#include <vector>

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
};

class Framework : public virtual Bundle {
public:
  virtual void Init() = 0;
};

} // namespace osgi

namespace posgi {

class BundleImpl;
class FrameworkImpl;

class BundleContextImpl : public osgi::BundleContext {
public:
  BundleContextImpl(BundleImpl *bundleImpl);
  BundleContextImpl(FrameworkImpl *frameworkImpl);
  osgi::Bundle *InstallBundle(std::string manifest,
                              osgi::BundleActivator *activator = nullptr);

private:
  FrameworkImpl *frameworkImpl;
  BundleImpl *bundleImpl;
};

class BundleImpl : public virtual osgi::Bundle {
public:
  void Start() {
    std::cout << "BundleImpl::Start (" << this->id << ", "
              << this->bundleSymbolicName << ", " << this->activatorRef << ")"
              << std::endl;
    if (this->activatorRef != nullptr) {
      std::cout << "BundleImpl::Start::Activator (" << this->id << ", "
                << this->bundleSymbolicName << ", " << this->activatorRef << ")"
                << std::endl;
      this->activatorRef->Start(this->bc);
    }
  }
  void Stop() {
    std::cout << "BundleImpl::Stop (" << this->id << ", "
              << this->bundleSymbolicName << ")" << std::endl;
    if (this->activatorRef != nullptr) {
      std::cout << "BundleImpl::Start::Activator (" << this->id << ", "
                << this->bundleSymbolicName << ")" << std::endl;
      this->activatorRef->Stop(this->bc);
    }
  }
  std::string GetSymbolicName() {
    std::cout << "BundleImpl::GetSymbolicName" << std::endl;
    return this->bundleSymbolicName;
  }
  osgi::BundleContext *GetBundleContext() {
    std::cout << "BundleImpl::GetBundleContext" << std::endl;
    return bc;
  };
  void SetBundleContext(BundleContextImpl *bundleContextImpl) {
    std::cout << "BundleImpl::SetBundleContext" << std::endl;
    this->bc = bundleContextImpl;
  };

public:
  // TODO make all private with setters, or use friends
  BundleContextImpl *bc;
  int id;
  std::string bundleSymbolicName;
  osgi::BundleActivator *activatorRef = nullptr;
};

class FrameworkImpl : public BundleImpl, public osgi::Framework {
public:
  void Init() {
    std::cout << "FrameworkImpl::Init" << std::endl;
    this->id = 0;
    this->bundleSymbolicName = "org.posgi.framework.impl.FrameworkImpl";
    BundleContextImpl *bc = new BundleContextImpl(this);
    this->SetBundleContext(bc);
    this->bundles = std::vector<BundleImpl *>();
  }
  osgi::Bundle *InstallBundle(std::string manifest,
                              osgi::BundleActivator *activator = nullptr) {
    std::cout << "FrameworkImpl::InstallBundle" << std::endl;
    BundleImpl *bundleImpl = new BundleImpl();
    // inc bundle id
    bundleImpl->id = ++lastBundleId;
    bundleImpl->bundleSymbolicName = manifest;
    if (activator != nullptr) {
      bundleImpl->activatorRef = activator;
    }
    std::cout << "FrameworkImpl::InstallBundle " << bundleImpl << std::endl;
    BundleContextImpl *bundleContextImpl = new BundleContextImpl(bundleImpl);
    bundleImpl->SetBundleContext(bundleContextImpl);
    bundles.push_back(bundleImpl);
    return bundleImpl;
  }
  void dump_bundles() {
    std::cout << "FrameworkImpl::dump_bundles()" << std::endl;
    for (const auto b : bundles) {
      std::cout << "  " << b->bundleSymbolicName << std::endl;
    }
  }

private:
  std::vector<BundleImpl *> bundles;
  int lastBundleId = 0;
};

BundleContextImpl::BundleContextImpl(BundleImpl *bundleImpl) {
  std::cout << "BundleContextImpl::BundleContextImpl(BundleImpl)" << std::endl;
  this->bundleImpl = bundleImpl;
}
BundleContextImpl::BundleContextImpl(FrameworkImpl *frameworkImpl) {
  std::cout << "BundleContextImpl::BundleContextImpl(FrameworkImpl)"
            << std::endl;
  this->frameworkImpl = frameworkImpl;
  // preserve as well as bundle context
  // TODO cast problems
  // this->bundleImpl = (BundleImpl)frameworkImpl;
}
osgi::Bundle *
BundleContextImpl::InstallBundle(std::string manifest,
                                 osgi::BundleActivator *activator) {
  // should never be null
  std::cout << "BundleContextImpl::InstallBundle" << std::endl;
  FrameworkImpl *fwImpl = this->frameworkImpl;
  return fwImpl->InstallBundle(manifest, activator);
}

} // namespace posgi

namespace sample {

class SomeBundle : public osgi::BundleActivator {
public:
  SomeBundle(std::string name) {
    std::cout << "SomeBundle::SomeBundle(" << name << ")" << std::endl;
    this->name = name;
  }
  void Start(osgi::BundleContext *bundleContext) {
    std::cout << name << "::Start" << std::endl;
  }
  void Stop(osgi::BundleContext *bundleContext) {
    std::cout << name << "::Stop" << std::endl;
  }

private:
  std::string name;
};

} // namespace sample

int main() {
  std::cout << "main" << std::endl;
  osgi::Framework *fw = new posgi::FrameworkImpl();
  fw->Init();
  fw->Start();
  osgi::BundleContext *bc = fw->GetBundleContext();
  osgi::Bundle *bundleA =
      bc->InstallBundle("BundleA", new sample::SomeBundle("BundleA"));
  osgi::Bundle *bundleB =
      bc->InstallBundle("BundleB", new sample::SomeBundle("BundleB"));
  osgi::Bundle *bundleC = bc->InstallBundle("BundleC");
  bundleA->Start();
  bundleB->Start();
  bundleC->Start();
  // TODO try out a dynamic cast
  dynamic_cast<posgi::FrameworkImpl *>(fw)->dump_bundles();
  bundleC->Stop();
  bundleB->Stop();
  bundleA->Stop();
  fw->Stop();

  return 0;
}
