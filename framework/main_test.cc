#include <gtest/gtest.h>

#include "posgi.h"

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(MyTestSuite, MyTestCase) {
  int x = 5;
  int y = 10;
  ASSERT_GT(y, x);
  ASSERT_EQ(y - x, 5);
}

TEST(MyTestSuite, main_version) {
  int rc = do_main(2, (char*[]){"main", "--version"});
  ASSERT_EQ(rc, 1);
}
