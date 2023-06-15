#include "org/posgi/framework/impl/utils/system_utils.h"

#include <sys/utsname.h>

#include <string>

namespace Utils {

std::string SystemUtils::get_os() {
  std::string os;
  struct utsname osInfo {};
  if (uname(&osInfo) == 0) {
    os = std::string(osInfo.sysname) + " " + std::string(osInfo.release);
  } else {
    os = std::string("unknown os and version");
  }
  return os;
}

}  // namespace Utils
