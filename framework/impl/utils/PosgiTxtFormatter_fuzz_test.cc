#include <assert.h>
#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include "org/posgi/framework/impl/manifest_parser.h"
#include "org/posgi/framework/impl/utils/PosgiTxtFormatter.h"

FUZZ_TEST_SETUP() {
  // enable if you want to see fuzzed data
  // plog::init(plog::fatal, "posgi_fuzz_tests_txtformatter.log");
  PLOG_FATAL << "FUZZ_TEST_SETUP";
}

FUZZ_TEST(const uint8_t *data, size_t size) {
  FuzzedDataProvider fuzzed_data(data, size);
  // std::string text_corpus = fuzzed_data.ConsumeRandomLengthString();

  constexpr int max_bytes = 25;
  std::string text_corpus = fuzzed_data.ConsumeBytesAsString(max_bytes);
  text_corpus.erase(std::remove(text_corpus.begin(), text_corpus.end(), '\n'),
                    text_corpus.end());

  constexpr int max_max_width = 25;
  auto max_width_corpus =
      fuzzed_data.ConsumeIntegralInRange<size_t>(0, max_max_width);
  constexpr int max_min_left = 25;
  auto min_left_char_corpus =
      fuzzed_data.ConsumeIntegralInRange<size_t>(0, max_min_left);

  PLOG_FATAL << "FUZZ_TEST: '" << text_corpus << "', " << max_width_corpus
             << ", " << min_left_char_corpus;
  auto formatter = Utils::PosgiTxtFormatter();
  formatter.fixed_column(text_corpus, max_width_corpus, min_left_char_corpus);
}
