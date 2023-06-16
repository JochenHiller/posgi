#include "commands_framework.h"

#include <plog/Log.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "command_lb.h"
#include "org/osgi/framework/bundle.h"
#include "org/osgi/framework/bundle_context.h"
#include "org/osgi/service/console/command.h"

namespace osgi {

osgi::Bundle* getBundleFromSecondArg(
    const std::vector<std::string>& command_args,
    BundleContext* bundleContext) {
  if (command_args.size() < 2) {
    std::cout << "Please specify bundle id" << std::endl;
    return nullptr;
  } else {
    std::string bundleId = command_args.at(1);
    if (bundleId.empty()) {
      std::cout << "Please specify bundle id" << std::endl;
      return nullptr;
    }
    std::string str = command_args.at(1);
    try {
      int bundleIdNum = std::stoi(str);
      auto bundle = bundleContext->GetBundle(bundleIdNum);
      if (bundle != nullptr) {
        return bundle;
      } else {
        std::cout << "Bundle not found: " << bundleIdNum << std::endl;
        return nullptr;
      }
    } catch (const std::invalid_argument& ia) {
      std::cout << "Invalid bundle id (not a number): " << str << std::endl;
      return nullptr;
    } catch (const std::out_of_range& oor) {
      std::cout << "Invalid bundle id (out of range): " << str << std::endl;
      return nullptr;
    }
  }
}

CommandStartBundle::CommandStartBundle(BundleContext* bc) : bundleContext_(bc) {
  PLOG_INFO << "CommandStartBundle::CommandBundleStart";
}

std::string CommandStartBundle::scope() {
  return "posgi";
}

std::string CommandStartBundle::function() {
  return "start";
}

std::string CommandStartBundle::help_short() {
  return "Starts a bundle";
}

std::string CommandStartBundle::help() {
  return R"(Starts a bundle
  Arguments: <bundle-id>
)";
}

void CommandStartBundle::execute(std::vector<std::string> args) {
  PLOG_INFO << "CommandStartBundle::execute" << args;
  auto bundle = getBundleFromSecondArg(args, bundleContext_);
  if (bundle != nullptr) {
    if (bundle->GetState() == Bundle::ACTIVE) {
      std::cout << "Bundle already active" << std::endl;
    } else {
      bundle->Start();
    }
  }
}

CommandStopBundle::CommandStopBundle(BundleContext* bc) : bundleContext_(bc) {
  PLOG_INFO << "CommandStopBundle::CommandStopBundle";
}

std::string CommandStopBundle::scope() {
  return "posgi";
}

std::string CommandStopBundle::function() {
  return "stop";
}

std::string CommandStopBundle::help_short() {
  return "Stops a bundle";
}

std::string CommandStopBundle::help() {
  return R"(Stops a bundle
  Arguments: <bundle-id>
)";
}

void CommandStopBundle::execute(std::vector<std::string> args) {
  PLOG_INFO << "CommandStopBundle::execute" << args;
  auto bundle = getBundleFromSecondArg(args, bundleContext_);
  if (bundle != nullptr) {
    if (bundle->GetState() == Bundle::RESOLVED) {
      std::cout << "Bundle already stopped" << std::endl;
    } else {
      bundle->Stop();
    }
  }
}

CommandExitFramework::CommandExitFramework(BundleContext* bc)
    : bundleContext_(bc) {
  PLOG_INFO << "CommandExitFramework::CommandExitFramework";
}

std::string CommandExitFramework::scope() {
  return "posgi";
}

std::string CommandExitFramework::function() {
  return "exit";
}

std::string CommandExitFramework::help_short() {
  return "Exit OSGi framework";
}

std::string CommandExitFramework::help() {
  return R"(Exit OSGi framework
)";
}

void CommandExitFramework::execute(std::vector<std::string> args) {
  PLOG_INFO << "CommandExitFramework::execute" << args;
  // TODO(jhi): does not work on Linux
  this->bundleContext_->GetBundle(0)->Stop();
  std::cout << "OSGi framework terminated" << std::endl;
}

}  // end namespace osgi
