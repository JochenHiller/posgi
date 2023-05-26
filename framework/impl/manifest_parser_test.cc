#include "org/posgi/framework/impl/manifest_parser.h"

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include "gtest/gtest.h"

// TODO(JochenHiller): enable logging during tests
// TODO(JochenHiller): use a common fixture, see
// https://google.github.io/googletest/primer.html

// use a fixture, use TEST_F then later
class ManifestParserTest : public testing::TestWithParam<std::string> {
 protected:
  void SetUp() override {
    plog::init(plog::debug, "posgi.log");
    PLOG_ERROR << "test";
    parser = new posgi::ManifestParser();
  }
  posgi::ManifestParser *parser;
};
class OneLineValidTest : public ManifestParserTest {};
class OneLineInvalidTest : public ManifestParserTest {};

// NOLINTNEXTLINE
TEST_F(ManifestParserTest, parse_standard) {
  std::map<std::string, std::string> headers = parser->parse(
      "Bundle-SymbolicName: org.posgi.framework\n"
      "Bundle-Version: 1.0.0\n"
      "Bundle-Activator: org.posgi.framework.impl.Activator\n"
      "Import-Package: A,B,C\n"
      "Export-Package: X,Y,Z\n");
  ASSERT_EQ(5, headers.size());
  EXPECT_EQ("org.posgi.framework", headers["Bundle-SymbolicName"]);
  EXPECT_EQ("1.0.0", headers["Bundle-Version"]);
  EXPECT_EQ("org.posgi.framework.impl.Activator", headers["Bundle-Activator"]);
  EXPECT_EQ("A,B,C", headers["Import-Package"]);
  EXPECT_EQ("X,Y,Z", headers["Export-Package"]);
}

// NOLINTNEXTLINE
TEST_F(ManifestParserTest, parse_empty) {
  EXPECT_EQ(0, parser->parse("").size());
  EXPECT_EQ(0, parser->parse("   ").size());
  EXPECT_EQ(0, parser->parse("  \n  ").size());
}

// NOLINTNEXTLINE
TEST_F(ManifestParserTest, parse_one_line_valid) {
  std::map<std::string, std::string> headers;

  headers = parser->parse("Manifest-Version:1.0");
  ASSERT_EQ(1, headers.size());
  EXPECT_EQ("1.0", headers["Manifest-Version"]);

  headers = parser->parse("Manifest-Version:  1.0\n");
  ASSERT_EQ(1, headers.size());
  EXPECT_EQ("1.0", headers["Manifest-Version"]);
}

// NOLINTNEXTLINE
TEST_P(OneLineValidTest, parse_one_line_valid) {
  std::map<std::string, std::string> headers;
  PLOG_ERROR << "parse_one_line_invalid_parameter:" << GetParam();
  headers = parser->parse(GetParam());
  ASSERT_EQ(1, headers.size());
}

// these values are all valid
// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(ManifestParserTest, OneLineValidTest,
                         testing::Values("Key:Value", "Key   :    Value",
                                         "Key: Value", "Key: Value"));

// NOLINTNEXTLINE
TEST_P(OneLineInvalidTest, parse_one_line_invalid) {
  std::map<std::string, std::string> headers;
  PLOG_ERROR << "parse_one_line_invalid_parameter:" << GetParam();
  headers = parser->parse(GetParam());
  ASSERT_EQ(0, headers.size());
}

// these values are all invalid
// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    ManifestParserTest, OneLineInvalidTest,
    testing::Values("KeyOnly", ": MissingKey", "MissingValue:",
                    "MissingValueWithTrailingBlank :", "Key:Value1:Value2",
                    " HeaderWithLeadingBlank: value"));
