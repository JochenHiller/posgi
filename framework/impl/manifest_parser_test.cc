#include "org/posgi/framework/impl/manifest_parser.h"

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "gtest/gtest.h"
#include "org/posgi/framework/impl/utils/file_utils.h"

// use a fixture, use TEST_F or TEST_P for paramterized tests then later
class ManifestParserTest : public testing::TestWithParam<std::string> {
 protected:
  void SetUp() override {
    // enable verbose logging for testing flatten()
    plog::init(plog::debug, "posgi_tests.log");
    PLOG_INFO << "SetUp";
    parser = new posgi::ManifestParser();
  }
  void TearDown() override {
    PLOG_INFO << "TearDown";
    delete parser;
  }

  void test_one_file(std::string filename, int expected_size) {
    PLOG_ERROR << "test_one_file: " << filename;
    std::string file_content = Utils::FileUtils::read_file_content(filename);
    PLOG_ERROR << "parse_test_files_valid: ";
    PLOG_ERROR << "===";
    PLOG_ERROR << file_content;
    PLOG_ERROR << "===";
    auto headers = parser->parse(file_content);
    ASSERT_EQ(headers.size(), expected_size);
  }

  // TODO(jhi): how to know where ../ is, as called from build
  // directory
  const std::string rootDir = "../";
  posgi::ManifestParser* parser;
  std::map<std::string, std::string> headers;
};

class OneLineValidTest : public ManifestParserTest {};
class OneLineInvalidTest : public ManifestParserTest {};

// NOLINTNEXTLINE
TEST_F(ManifestParserTest, flatten) {
  EXPECT_EQ("K: V,V2", parser->flatten("K: V\n ,V2"));
  EXPECT_EQ("K: V,V2", parser->flatten("K: V\r\n ,V2"));
  EXPECT_EQ("K12345: V12345", parser->flatten("K12345: V1\n 2345"));
  EXPECT_EQ("K: V,V2", parser->flatten("\nK: V\n ,V2"));
  EXPECT_EQ("K: V,V2", parser->flatten("\nK: V\n ,V2\n"));
  EXPECT_EQ("K: V,V2", parser->flatten("\nK: V\n\n ,V2\n"));

  auto filename = rootDir + "framework/impl/testdata/manifest_felix.mf";
  auto content = Utils::FileUtils::read_file_content(filename);
  auto s = parser->flatten(content);
}

// NOLINTNEXTLINE
TEST_F(ManifestParserTest, parse_standard) {
  headers = parser->parse(
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
TEST_F(ManifestParserTest, parse_simple_valid) {
  headers = parser->parse("Manifest-Version:1.0");
  // assert will stop on fail
  ASSERT_EQ(1, headers.size());
  // expect will continue on fail
  EXPECT_EQ("1.0", headers["Manifest-Version"]);

  headers = parser->parse("Manifest-Version:  1.0\n");
  ASSERT_EQ(1, headers.size());
  EXPECT_EQ("1.0", headers["Manifest-Version"]);
}

// NOLINTNEXTLINE
TEST_F(ManifestParserTest, parse_valid_felix) {
  // a test can be skipped by add GTEST_SKIP() << "reason";
  // GTEST_SKIP() << "WIP test";

  auto filename = rootDir + "framework/impl/testdata/manifest_felix.mf";
  auto content = Utils::FileUtils::read_file_content(filename);
  headers = parser->parse(content);
  EXPECT_EQ(17, headers.size());

  for (const auto& pair : headers) {
    PLOG_ERROR << pair.first << ": " << pair.second << std::endl;
  }
}

// NOLINTNEXTLINE
TEST_P(OneLineValidTest, parse_one_line_valid) {
  PLOG_ERROR << "parse_one_line_invalid_parameter:" << GetParam();
  headers = parser->parse(GetParam());
  // valid means we expect 1 header
  ASSERT_EQ(1, headers.size());
}

// these values are all valid
// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(ManifestParserTest, OneLineValidTest,
                         testing::Values("Key:Value", "Key   :    Value",
                                         "Key: Value", "Key: Value",
                                         "Key:Value1:Value2"));

// NOLINTNEXTLINE
TEST_P(OneLineInvalidTest, parse_one_line_invalid) {
  PLOG_ERROR << "parse_one_line_invalid_parameter:" << GetParam();
  headers = parser->parse(GetParam());
  // invalid means we expect 0 header
  ASSERT_EQ(0, headers.size());
}

// TODO(jhi): what to do with " KeyWithLeadingBlank: value" ?

// these values are all invalid
// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(ManifestParserTest, OneLineInvalidTest,
                         testing::Values("KeyOnly", ": MissingKey",
                                         "MissingValue:",
                                         "MissingValueWithKeyTrailingBlank :"));

// NOLINTNEXTLINE
TEST_F(ManifestParserTest, parse_test_files_valid) {
  auto testfiles = Utils::FileUtils::search_files(
      rootDir + "framework/impl/testdata", "manifest_valid.*\\.mf");

  for (const auto& filename : testfiles) {
    test_one_file(filename, 1);
  }
}

// NOLINTNEXTLINE
TEST_F(ManifestParserTest, parse_test_files_invalid) {
  auto testfiles = Utils::FileUtils::search_files(
      rootDir + "framework/impl/testdata", "manifest_invalid.*\\.mf");

  for (const auto& filename : testfiles) {
    test_one_file(filename, 0);
  }
}
