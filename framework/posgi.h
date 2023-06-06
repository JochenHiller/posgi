#ifndef POSGI_POSGI_H
#define POSGI_POSGI_H

#include <string>
#include <vector>

// TODO(jhi): enums?
constexpr int RC_OK = 0;
constexpr int RC_VERSION = 1;
constexpr int RC_HELP = 2;

int do_main(std::vector<std::string> args);

#endif  // POSGI_POSGI_H
