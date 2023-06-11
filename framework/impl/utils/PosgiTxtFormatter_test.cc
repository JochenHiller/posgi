#include "org/posgi/framework/impl/utils/PosgiTxtFormatter.h"

#include <gtest/gtest.h>

#include <string>

// NOLINTNEXTLINE
TEST(PosgiTxtFormatterTest, fixed_column_valid_args) {
  const std::string pattern_long = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  auto formatter = Utils::PosgiTxtFormatter();

  // input <= max_width
  EXPECT_EQ("12345     ", formatter.fixed_column("12345", 10, 4));
  EXPECT_EQ("1234567890", formatter.fixed_column("1234567890", 10, 4));
  EXPECT_EQ("1 ", formatter.fixed_column("1", 2, 2));
  EXPECT_EQ("  ", formatter.fixed_column("", 2, 2));

  // input > max_width
  EXPECT_EQ("1234..WXYZ", formatter.fixed_column(pattern_long, 10, 4));
  EXPECT_EQ("1234..890A", formatter.fixed_column("1234567890A", 10, 4));
  EXPECT_EQ("1234..90AB", formatter.fixed_column("1234567890AB", 10, 4));
  EXPECT_EQ("1234..0ABC", formatter.fixed_column("1234567890ABC", 10, 4));

  // input > max_width
  EXPECT_EQ("12..UVWXYZ", formatter.fixed_column(pattern_long, 10, 2));
  EXPECT_EQ("12..67890A", formatter.fixed_column("1234567890A", 10, 2));
  EXPECT_EQ("12..7890AB", formatter.fixed_column("1234567890AB", 10, 2));
  EXPECT_EQ("12..890ABC", formatter.fixed_column("1234567890ABC", 10, 2));

  // min_left_chars close to max_width
  EXPECT_EQ("12345678..", formatter.fixed_column(pattern_long, 10, 10));
  EXPECT_EQ("12345678..", formatter.fixed_column(pattern_long, 10, 9));
  EXPECT_EQ("12345678..", formatter.fixed_column(pattern_long, 10, 8));
  EXPECT_EQ("1234567..Z", formatter.fixed_column(pattern_long, 10, 7));

  // max_width close to text size
  EXPECT_EQ("1234567890ABCDEFGHIJKLMNOPQRSTUVW..",
            formatter.fixed_column(pattern_long, pattern_long.size() - 1,
                                   pattern_long.size() - 1));
  EXPECT_EQ("1234567890ABCDEFGHIJKLMNOPQRSTUVW..",
            formatter.fixed_column(pattern_long, pattern_long.size() - 1,
                                   pattern_long.size() - 2));
  EXPECT_EQ("1234567890ABCDEFGHIJKLMNOPQRSTUVW..",
            formatter.fixed_column(pattern_long, pattern_long.size() - 1,
                                   pattern_long.size() - 3));
  EXPECT_EQ("1234567890ABCDEFGHIJKLMNOPQRSTUV..Z",
            formatter.fixed_column(pattern_long, pattern_long.size() - 1,
                                   pattern_long.size() - 4));
}

// NOLINTNEXTLINE
TEST(PosgiTxtFormatterTest, fixed_column_egde_cases) {
  const std::string pattern_long = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  auto formatter = Utils::PosgiTxtFormatter();

  // min_left_chars > max_width
  EXPECT_EQ("12345678..", formatter.fixed_column(pattern_long, 10, 20));
  EXPECT_EQ("12345678..", formatter.fixed_column(pattern_long, 10, 11));

  // args == 0
  EXPECT_EQ("", formatter.fixed_column(pattern_long, 0, 1));
  EXPECT_EQ("", formatter.fixed_column("", 0, 1));
  EXPECT_EQ("1", formatter.fixed_column(pattern_long, 1, 0));
  EXPECT_EQ(" ", formatter.fixed_column("", 1, 0));

  // auto type conversion: -1 will be (unsigned) INT_MAX
  EXPECT_EQ("12345678..", formatter.fixed_column(pattern_long, 10, -1));
  EXPECT_EQ(pattern_long + std::string(16, ' '),
            formatter.fixed_column(pattern_long, -1, 4));
}
