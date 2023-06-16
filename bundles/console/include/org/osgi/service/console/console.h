#ifndef POSGI_ORG_OSGI_SERVICE_CONSOLE_CONSOLE_H
#define POSGI_ORG_OSGI_SERVICE_CONSOLE_CONSOLE_H

#include <vector>

#include "org/osgi/service/console/command.h"

// TODO(jhi): osgi::service namespace?
namespace osgi {

class Console {
 public:
  std::vector<osgi::Command*> getCommands();
};

}  // end namespace osgi

#endif  // POSGI_ORG_OSGI_SERVICE_CONSOLE_CONSOLE_H
