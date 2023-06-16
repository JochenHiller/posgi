#include "osgi_console.h"

#include <plog/Log.h>

#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#include "command_lb.h"
#include "commands_framework.h"
#include "org/osgi/framework/bundle.h"
#include "org/osgi/service/console/console.h"

namespace osgi {

void OsgiConsole::Start(osgi::BundleContext* bundleContext) {
  PLOG_INFO << "OsgiConsole::Start";
  this->bundleContext_ = bundleContext;

  // all all known commands, hard coded for the moment
  commands_.push_back(new CommandListBundles(bundleContext));
  commands_.push_back(new CommandStartBundle(bundleContext));
  commands_.push_back(new CommandStopBundle(bundleContext));
  commands_.push_back(new CommandExitFramework(bundleContext));

  consoleThread_ = new std::thread(&OsgiConsole::console, this);
}
void OsgiConsole::Stop(osgi::BundleContext* bundleContext) {
  PLOG_INFO << "OsgiConsole::Stop";
  this->bundleContext_ = nullptr;
}

std::vector<Command*> OsgiConsole::getCommands() {
  return commands_;
}

Command* OsgiConsole::findCommand(const std::string& scope_function_to_search) {
  constexpr char delimiter = ':';
  std::vector<Command*> results;
  std::copy_if(commands_.begin(), commands_.end(), std::back_inserter(results),
               [scope_function_to_search](Command* command) {
                 std::string command_scope_function =
                     command->scope() + delimiter + command->function();
                 if (command_scope_function == scope_function_to_search) {
                   return true;
                 }
                 if (command->function() == scope_function_to_search) {
                   return true;
                 }
                 return false;
               });
  if (results.size() == 1) {
    return results.at(0);
  } else if (results.size() == 0) {
    return nullptr;
  } else {
    PLOG_WARNING << "Duplicate commands found";
    // TODO(jhi): what to do if we have multiple commands? At the moment do not
    // support that.
    return nullptr;
  }
}

void OsgiConsole::show_help() {
  // Use multiline string supported since C++11
  std::string usage = R"(Available builtin commands:
  help                  Show this help
  help <command>        Show specific help for given command
  quit                  Quit OSGi console

Available registered commands:
)";
  std::ostringstream oss;
  oss << usage;
  std::for_each(commands_.begin(), commands_.end(), [&oss](Command* cmd) {
    oss << "  " << cmd->scope() << ":" << cmd->function() << "\t\t"
        << cmd->help_short() << std::endl;
  });
  std::cout << oss.str() << std::endl;
}

void OsgiConsole::show_help_for_command(std::vector<std::string> args) {
  if (args.size() != 2) {
    std::cout << "Usage: help <command>" << std::endl;
    return;
  }
  std::string command = args.at(1);
  // builtin commands
  if (command == "help") {
    std::cout << "Show help" << std::endl;
  } else if (command == "quit") {
    std::cout << "Quit OSGi console" << std::endl;
  } else {
    Command* found_command = findCommand(command);
    if (found_command != nullptr) {
      std::cout << found_command->help() << std::endl;
    } else {
      std::cout << "Unknown command: " << command << std::endl;
    }
  }
}

// Split the command by the delimiter " " and trim all args
std::vector<std::string> parseString(const std::string& str) {
  std::vector<std::string> elements;
  std::istringstream iss(str);
  std::string element;

  while (iss >> element) {
    elements.push_back(element);
  }

  return elements;
}

void OsgiConsole::console() {
  while (true) {
    std::string command_line;
    // Prompt for command
    std::cout << "osgi> ";

    // Read command from stdin
    std::getline(std::cin, command_line);
    std::vector<std::string> command_args = parseString(command_line);

    std::string command;
    if (command_args.size() == 0) {
      continue;
    } else {
      command = command_args.at(0);
    }

    // builtin commands
    if (command == "help") {
      if (command_args.size() == 1) {
        show_help();
      } else {
        show_help_for_command(command_args);
      }
    } else if (command == "quit") {
      std::cout << "OSGi console terminated" << std::endl;
      break;  // leave loop, end thread
    } else {
      Command* found_command = findCommand(command);
      if (found_command != nullptr) {
        found_command->execute(command_args);
        continue;
      } else {
        if (command != "") {
          std::cout << "Unknown command: " << command << std::endl;
        }
      }
    }
  }
}

}  // end namespace osgi
