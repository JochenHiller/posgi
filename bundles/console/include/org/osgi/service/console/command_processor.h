#ifndef POSGI_ORG_OSGI_SERVICE_CONSOLE_COMMAND_PROCESSOR_H
#define POSGI_ORG_OSGI_SERVICE_CONSOLE_COMMAND_PROCESSOR_H

#include <iostream>
#include <string>

#include "org/osgi/service/console/command_session.h"

namespace osgi {

// see
// https://github.com/apache/felix-dev/blob/master/gogo/runtime/src/main/java/org/apache/felix/service/command/CommandProcessor.java
class CommandProcessor {
 public:
  // This are property names in service registration.
  static const std::string COMMAND_SCOPE = "osgi.command.scope";
  static const std::string COMMAND_FUNCTION = "osgi.command.function";

  virtual CommandSession createSession(std::istream in, std::ostream out,
                                       std::ostream err) = 0;
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_SERVICE_CONSOLE_COMMAND_PROCESSOR_H
