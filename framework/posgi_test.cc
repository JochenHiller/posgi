#include "posgi.h"

#include "gtest/gtest.h"

// TODO(JochenHiller): why does clang-tidy complain about gtest?
// NOLINTNEXTLINE
TEST(Posgi, do_main_version) {
  auto rc = do_main({"main", "--version"});
  ASSERT_EQ(RC_VERSION, rc);
}

TEST(Posgi, do_main_help) {
  auto rc = do_main({"main", "--help"});
  ASSERT_EQ(RC_HELP, rc);
  // help is more important than version
  rc = do_main({"main", "--version", "--help"});
  ASSERT_EQ(RC_HELP, rc);
  rc = do_main({"main", "--help", "--version"});
  ASSERT_EQ(RC_HELP, rc);
}