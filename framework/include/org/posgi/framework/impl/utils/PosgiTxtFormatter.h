#pragma once
#include <plog/Record.h>
#include <plog/Util.h>

#include <iomanip>
#include <iostream>

namespace Utils {

// clang-format off
/*
 * This class will format a log message to this format:

<Timestamp (ISO8601)> " " <LogLevel> " " [<thread-id-or-name>] [<file-class-method@line>] msg

Sample:

2023-05-18T12:41:14.082+02:00 ERROR [9562345] [posg..class.method@line] log message

 * where:
 *   ISO8601 timestamp:          see https://en.wikipedia.org/wiki/ISO_8601
 *   [<thread-id-or-name>]:      shortened to max 15 chars
 *   [<file-class-method@line>]: shortened to max 25 chars
*/
// clang-format on

template <bool useUtcTime>
class PosgiTxtFormatterImpl {
 public:
  static plog::util::nstring header() {
    return plog::util::nstring();
  }

  // clang-format off
  /*
   * This method will format a column to a fixed size. To give reasonable text,
   * it can prefix some chars with "min_left_chars", add some dots to indicate
   * more, and suffix the rest of the text from the right. So the line
   * number and function name will be visible in most cases.
   *
   * The following code has been generated by ChatGPT using this prompt:

Generate a C++ function `std::string fixed_column(std::string text, size_t max_width, size_t min_left_chars)`
which returns a string according to these rules:
- the output is max_width large
- the output will be padded with blanks on right side if test is short than text size
- the output starts with min_left characters
- the output will add `..` to indicate more characters
- the output will add remaining characters, from text right side

  * The code was 80% correct, I had to fix some bugs for edge cases in input data.
  * There is also a fuzz test to check about different input data.
  */
  // clang-format on

  static constexpr int max_padding = 16;
  static plog::util::nstring fixed_column(std::string text, size_t max_width,
                                         size_t min_left_chars) {
    if (text.length() <= max_width) {
      auto padding = max_width - text.length();
      // limit padding in case max_width is too high
      if (padding > max_padding) {
        padding = max_padding;
      }
      return text + std::string(padding, ' ');  // Pad with blanks
    }

    if (min_left_chars >= max_width) {
      // fix: had to subtract -2 to limit the output to max_width
      auto number_of_dots = 2;
      if (max_width < number_of_dots) {
        number_of_dots = max_width;
      }
      return text.substr(0, max_width - number_of_dots) +
             std::string(number_of_dots, '.');  // Add ".." indicator
    }

    if (max_width == 1) {
      return text.substr(0, 1);
    } else if (max_width == 2) {
      return text.substr(0, 2);
    } else {
      int remaining_chars =
          max_width - min_left_chars - 2;  // "- 2" to account for ".."
      // fix: check if remaining_chars is too small which will result in wrong
      // access to string
      if (remaining_chars < 0) {
        remaining_chars = 0;
        min_left_chars = max_width - 2;
        if (min_left_chars < 0) {
          min_left_chars = 0;
        }
      }
      return text.substr(0, min_left_chars) + ".." +
             text.substr(text.length() - remaining_chars);
    }
  }

  static plog::util::nstring format(const plog::Record& record) {
    tm t;
    useUtcTime ? plog::util::gmtime_s(&t, &record.getTime().time)
               : plog::util::localtime_s(&t, &record.getTime().time);

    plog::util::nostringstream ss;

    ss << t.tm_year + 1900 << "-" << std::setfill(PLOG_NSTR('0'))
       << std::setw(2) << t.tm_mon + 1 << PLOG_NSTR("-")
       << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday
       << PLOG_NSTR("T");
    ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour
       << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2)
       << t.tm_min << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0'))
       << std::setw(2) << t.tm_sec << PLOG_NSTR(".")
       << std::setfill(PLOG_NSTR('0')) << std::setw(3)
       << static_cast<int>(record.getTime().millitm);

    long offset = t.tm_gmtoff;
    ss << (offset >= 0 ? "+" : "-");
    ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << offset / 3600 << ":"
       << std::setfill(PLOG_NSTR('0')) << std::setw(2) << offset % 3600
       << PLOG_NSTR(" ");

    constexpr int min_left_chars = 2;
    constexpr int thread_width = 6;
    constexpr int func_line_width = 25;

    auto threadName = fixed_column(std::to_string(record.getTid()), thread_width,
                                  min_left_chars);
    auto funcLine = fixed_column(std::string(record.getFunc()) + PLOG_NSTR("@") +
                                    std::to_string(record.getLine()),
                                func_line_width, min_left_chars);

    ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left
       << severityToString(record.getSeverity()) << PLOG_NSTR(" ");
    ss << PLOG_NSTR("[") << threadName << PLOG_NSTR("] ");
    ss << PLOG_NSTR("[") << funcLine << PLOG_NSTR("] ");
    ss << record.getMessage() << PLOG_NSTR("\n");

    return ss.str();
  }
};

class PosgiTxtFormatter : public PosgiTxtFormatterImpl<false> {};
class PosgiTxtFormatterUtcTime : public PosgiTxtFormatterImpl<true> {};

}  // namespace Utils
