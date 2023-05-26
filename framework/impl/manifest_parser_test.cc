#include "org/posgi/framework/impl/manifest_parser.h"

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include "gtest/gtest.h"

// TODO(JochenHiller): enable logging during tests

// NOLINTNEXTLINE
TEST(ManifestParser, parse_standard) {
  posgi::ManifestParser *mp = new posgi::ManifestParser();
  std::map<std::string, std::string> headers = mp->parse(
      "Bundle-SymbolicName: org.posgi.framework\n"
      "Bundle-Version: 1.0.0\n"
      "Bundle-Activator: org.posgi.framework.impl.Activator\n"
      "Import-Package: org.osgi.framework.import\n"
      "Export-Package: org.posgi.framework.export\n");
  ASSERT_EQ(5, headers.size());
  ASSERT_EQ("org.posgi.framework", headers["Bundle-SymbolicName"]);
  ASSERT_EQ("1.0.0", headers["Bundle-Version"]);
  ASSERT_EQ("org.posgi.framework.impl.Activator", headers["Bundle-Activator"]);
  ASSERT_EQ("org.osgi.framework.import", headers["Import-Package"]);
  ASSERT_EQ("org.posgi.framework.export", headers["Export-Package"]);
}

// NOLINTNEXTLINE
TEST(ManifestParser, parse_empty) {
  posgi::ManifestParser *mp = new posgi::ManifestParser();
  ASSERT_EQ(0, mp->parse("").size());
  ASSERT_EQ(0, mp->parse("   ").size());
  ASSERT_EQ(0, mp->parse("  \n  ").size());
}

// NOLINTNEXTLINE
TEST(ManifestParser, parse_one_line_valid) {
  posgi::ManifestParser *mp = new posgi::ManifestParser();
  std::map<std::string, std::string> headers;

  headers = mp->parse("Manifest-Version:1.0");
  ASSERT_EQ(1, headers.size());
  ASSERT_EQ("1.0", headers["Manifest-Version"]);

  headers = mp->parse("Manifest-Version:  1.0\n");
  ASSERT_EQ(1, headers.size());
  ASSERT_EQ("1.0", headers["Manifest-Version"]);
}

// NOLINTNEXTLINE
TEST(ManifestParser, parse_one_line_invalid) {
  static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::debug, &consoleAppender);

  posgi::ManifestParser *mp = new posgi::ManifestParser();
  std::map<std::string, std::string> headers;

  headers = mp->parse("Manifest-Version");
  ASSERT_EQ(0, headers.size());

  headers = mp->parse(": 1.0");
  ASSERT_EQ(0, headers.size());
}
