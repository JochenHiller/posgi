#include <string>
#include <vector>

#include "posgi.h"  // NOLINT(build/include_subdir)

inline std::vector<std::string> convert_args(int argc, char *argv[]) {
  std::vector<std::string> args;
  for (int i = 0; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  return args;
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char *argv[]) {
  auto rc = do_main(convert_args(argc, argv));
  return static_cast<int>(rc);  // Convert enum to int
}
