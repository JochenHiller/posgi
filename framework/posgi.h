#ifndef POSGI_POSGI_H
#define POSGI_POSGI_H

#include <string>
#include <vector>

// return codes from main
enum class RC {
  kOk = 0,
  kVersion = 1,
  kHelp = 2,
};

RC do_main(std::vector<std::string> args);

#endif  // POSGI_POSGI_H
