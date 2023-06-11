#include <assert.h>
#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include "org/posgi/framework/impl/manifest_parser.h"

static posgi::ManifestParser *parser;

FUZZ_TEST_SETUP() {
  // enable if you want to see fuzzed data
  // plog::init(plog::fatal, "posgi_fuzz_tests_manifest.log");
  PLOG_FATAL << "FUZZ_TEST_SETUP";

  parser = new posgi::ManifestParser();
}

FUZZ_TEST(const uint8_t *data, size_t size) {
  FuzzedDataProvider fuzzed_data(data, size);
  std::string manifest_corpus = fuzzed_data.ConsumeRandomLengthString();

  PLOG_FATAL << "FUZZ_TEST: '" << manifest_corpus << "'";
  auto headers = parser->parse(manifest_corpus);
  // no good checks, as we do not know if the input is valid
  assert(headers.size() >= 0);
}
