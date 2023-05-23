#include "org/osgi/framework/service/osgi_console.h"

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

#include <plog/Log.h>

#include "org/osgi/framework/bundle.h"
#include "org/posgi/framework/impl/bundle_context_impl.h"

namespace osgi {

// TODO: move impl to another file and add an OSGi service with its own header
// file

osgi::Bundle *OsgiConsole::getBundleFromSecondArg(
    const std::vector<std::string> &command_args) {
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
      auto bundle = this->bundleContext->GetBundle(bundleIdNum);
      if (bundle != nullptr) {
        return bundle;
      } else {
        std::cout << "Bundle not found: " << bundleIdNum << std::endl;
        return nullptr;
      }
    } catch (const std::invalid_argument &ia) {
      std::cout << "Invalid bundle id (not a number): " << str << std::endl;
      return nullptr;
    } catch (const std::out_of_range &oor) {
      std::cout << "Invalid bundle id (out of range): " << str << std::endl;
      return nullptr;
    }
  }
}

void OsgiConsole::console() {
  while (true) {
    std::string command_line;
    // Prompt for command
    std::cout << "osgi> ";

    // Read command from stdin
    std::getline(std::cin, command_line);
    PLOG_INFO << "OsgiConsole::console: " << command_line;

    // Split the command by the delimiter ":"
    std::vector<std::string> command_args;
    boost::split(command_args, command_line, boost::is_any_of(" "));
    std::string command;
    if (command_args.size() == 0) {
      continue;
    } else {
      command = command_args.at(0);
    }

    // Check the command and perform corresponding actions
    //
    if (command == "help") {
      // Use multiline string supported since C++11
      std::string usage = R"(
Available commands
  help               Show this help
  lb                 List bundles
  start <bundle-id>  Start a bundle with given id
  stop <bundle-id>   Stop a bundle with given id
  quit               Quit OSGi console
  exit               Exit OSGi framework, means stop framework
      )";
      std::cout << usage << std::endl;
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
    } else if (command == "start") {
      auto bundle = getBundleFromSecondArg(command_args);
      if (bundle != nullptr) {
        bundle->Start();
      }
    } else if (command == "stop") {
      auto bundle = getBundleFromSecondArg(command_args);
      if (bundle != nullptr) {
        bundle->Stop();
      }
    } else {
      // std::cout << "Unknown command: " << command << std::endl;
    }
  }
}

OsgiConsole::OsgiConsole() { PLOG_INFO << "OsgiConsole::OsgiConsole"; }

void OsgiConsole::Start(osgi::BundleContext *bundleContext) {
  PLOG_INFO << "OsgiConsole::Start";
  this->bundleContext = bundleContext;
  consoleThread = new std::thread(&OsgiConsole::console, this);
}
void OsgiConsole::Stop(osgi::BundleContext *bundleContext) {
  PLOG_INFO << "OsgiConsole::Stop";
  this->bundleContext = nullptr;
}

} // namespace osgi