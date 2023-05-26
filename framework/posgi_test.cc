#include "posgi.h"

#include "gtest/gtest.h"

// TODO(JochenHiller): why does clang-tidy complain about gtest?
// NOLINTNEXTLINE
TEST(Posgi, do_main_version) {
  auto rc = do_main({"main", "--version"});
  ASSERT_EQ(rc, 1);
}
