#ifndef POSGI_ORG_OSGI_SERVICE_CONSOLE_COMMAND_H
#define POSGI_ORG_OSGI_SERVICE_CONSOLE_COMMAND_H

#include <string>
#include <vector>

#include "org/osgi/framework/bundle_context.h"

// TODO(jhi): osgi::service namespace?
namespace osgi {

/*
 * Sample: "psogi:lb <bundle-id> [<bundle-id>]*"
 */
class Command {
 public:
  virtual std::string scope() = 0;
  virtual std::string function() = 0;
  virtual std::string help_short() = 0;
  virtual std::string help() = 0;
  virtual void execute(std::vector<std::string> args) = 0;
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_SERVICE_CONSOLE_COMMAND_H
