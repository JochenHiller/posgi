#include "command_lb.h"

#include <plog/Log.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "org/osgi/framework/bundle.h"
#include "org/osgi/framework/bundle_context.h"
#include "org/osgi/service/console/command.h"

namespace osgi {

// plain old C-style printf
std::string bundle_to_string_c_plain(osgi::Bundle* bundle) {
  std::string format = "[%2d] %-10s %s";
  // https://clang.llvm.org/extra/clang-tidy/checks/cert/err33-c.html
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
  auto size = std::snprintf(nullptr, 0, format.c_str(), bundle->GetId(),
                            bundle->GetStateAsString().c_str(),
                            bundle->GetSymbolicName().c_str());
  std::string output(size + 1, '\0');
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
std::string bundle_to_string_cpp(osgi::Bundle* bundle) {
  constexpr int width_id = 2;
  constexpr int width_state = 10;
  std::ostringstream oss;
  oss << "[" << std::right << std::setw(width_id) << bundle->GetId() << "] ";
  oss << std::setw(width_state) << std::left << bundle->GetStateAsString()
      << " " << bundle->GetSymbolicName();
  std::string formattedString = oss.str();
  return formattedString;
}

CommandListBundles::CommandListBundles(BundleContext* bc) : bundleContext_(bc) {
  PLOG_INFO << "CommandListBundles::CommandListBundles";
}

std::string CommandListBundles::scope() {
  return "posgi";
}

std::string CommandListBundles::function() {
  return "lb";
}

std::string CommandListBundles::help_short() {
  return "List bundles and their state";
}

std::string CommandListBundles::help() {
  return R"(List bundles and their state
  Arguments: <bundle-id>
)";
}

void CommandListBundles::execute(std::vector<std::string> args) {
  PLOG_INFO << "CommandListBundles::execute" << args;
  std::cout << "List bundles" << std::endl;

  auto bundles = this->bundleContext_->GetBundles();
  std::cout << "  ID   State      Bundle-SymbolicName" << std::endl;
  std::cout << "  ---- ---------- -------------------" << std::endl;
  for (const auto b : *bundles) {
    std::string nice_format = bundle_to_string_cpp(b);
    std::cout << "  " << nice_format << std::endl;
  }
}

}  // end namespace osgi
