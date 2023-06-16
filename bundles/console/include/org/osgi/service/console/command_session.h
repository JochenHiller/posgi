#ifndef POSGI_ORG_OSGI_SERVICE_CONSOLE_COMMAND_SESSION_H
#define POSGI_ORG_OSGI_SERVICE_CONSOLE_COMMAND_SESSION_H

#include <string>

namespace osgi {

// see
// https://github.com/apache/felix-dev/blob/master/gogo/runtime/src/main/java/org/apache/felix/service/command/CommandSession.java
class CommandSession {
 public:
  virtual std::string execute(std::string commandline) = 0;
  virtual void close() = 0;
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_SERVICE_CONSOLE_COMMAND_SESSION_H
