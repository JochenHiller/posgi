#include "org/osgi/framework/service/osgi_console.h"

#include <atomic>
#include <chrono>              // NOLINT [build/c++11]
#include <condition_variable>  // NOLINT [build/c++11]
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>  // NOLINT [build/c++11]
#include <sstream>
#include <string>
#include <thread>  // NOLINT [build/c++11]
#include <vector>

#include "boost/algorithm/string.hpp"
#include "org/osgi/framework/bundle.h"
#include "org/posgi/framework/impl/bundle_context_impl.h"
#include "plog/Log.h"

namespace osgi {

// TODO(JochenHiller): move impl to another file and add an OSGi service with
// its own header file

// plain old C-style printf
std::string bundle_to_string_c_plain(osgi::Bundle *bundle) {
  std::string format = "[%2d] %-10s %s";
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
  auto size = std::snprintf(nullptr, 0, format.c_str(), bundle->GetId(),
                            bundle->GetStateAsString().c_str(),
                            bundle->GetSymbolicName().c_str());
  std::string output(size + 1, '\0');
  // https://clang.llvm.org/extra/clang-tidy/checks/cert/err33-c.html
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
  int ret = std::snprintf(&output[0], size + 1, format.c_str(), bundle->GetId(),
                          bundle->GetStateAsString().c_str(),
                          bundle->GetSymbolicName().c_str());
  if (ret < 0) {
    return "internal error happened" + std::to_string(ret);
  } else if (ret != size) {
    return "wrong nunber of characters written: expected " +
           std::to_string(size) + " got " + std::to_string(ret);
  } else {
    // all OK
    return output;
  }
}

// C++-style string stream
std::string bundle_to_string_cpp(osgi::Bundle *bundle) {
  constexpr int width_id = 2;
  constexpr int width_state = 10;
  std::ostringstream oss;
  oss << "[" << std::right << std::setw(width_id) << bundle->GetId() << "] ";
  oss << std::setw(width_state) << std::left << bundle->GetStateAsString()
      << " " << bundle->GetSymbolicName();
  std::string formattedString = oss.str();
  return formattedString;
}

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
      std::cout << "OSGi console terminated" << std::endl;
      break;
    } else if (command == "exit") {
      // TODO(JochenHiller): does not work on Linux
      this->bundleContext->GetBundle(0)->Stop();
      std::cout << "OSGi framework terminated" << std::endl;
    } else if (command == "lb") {
      std::cout << "List bundles" << std::endl;
      PLOG_ERROR << "OsgiConsole::console: " << this;
      PLOG_ERROR << "OsgiConsole::console: " << this->bundleContext;

      auto bundles = this->bundleContext->GetBundles();
      std::cout << "  ID   State      Bundle-SymbolicName" << std::endl;
      std::cout << "  ---- ---------- -------------------" << std::endl;
      for (const auto b : *bundles) {
        std::string nice_format = bundle_to_string_cpp(b);
        std::cout << "  " << nice_format << std::endl;
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
      if (command != "") {
        std::cout << "Unknown command: " << command << std::endl;
      }
    }
  }
}

OsgiConsole::OsgiConsole() {
  PLOG_INFO << "OsgiConsole::OsgiConsole";
  this->bundleContext = nullptr;
  this->consoleThread = nullptr;
}

void OsgiConsole::Start(osgi::BundleContext *bundleContext) {
  PLOG_INFO << "OsgiConsole::Start";
  this->bundleContext = bundleContext;
  consoleThread = new std::thread(&OsgiConsole::console, this);
}
void OsgiConsole::Stop(osgi::BundleContext *bundleContext) {
  PLOG_INFO << "OsgiConsole::Stop";
  this->bundleContext = nullptr;
}

}  // namespace osgi
