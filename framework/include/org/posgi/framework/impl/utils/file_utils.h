#ifndef POSGI_ORG_POSGI_FRAMEWORK_IMPL_UTIL_FILE_UTILS_H
#define POSGI_ORG_POSGI_FRAMEWORK_IMPL_UTIL_FILE_UTILS_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

namespace Utils {

class FileUtils {
 public:
  static std::vector<std::string> search_files(const std::string& directory,
                                               const std::string& pattern);
  static std::string read_file_content(std::string filename);
};

}  // namespace Utils

#endif  // POSGI_ORG_POSGI_FRAMEWORK_IMPL_UTIL_FILE_UTILS_H