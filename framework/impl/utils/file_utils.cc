#include "org/posgi/framework/impl/utils/file_utils.h"

#include <plog/Log.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

namespace Utils {

// since C++17
std::vector<std::string> FileUtils::search_files(const std::string& directory,
                                                 const std::string& pattern) {
  PLOG_ERROR << "search_files: directory " << directory;
  PLOG_ERROR << "search_files: pwd " << std::filesystem::current_path();
  std::regex regex(pattern);
  PLOG_ERROR << "search_files: regexp pattern " << pattern;

  std::vector<std::string> found_files = std::vector<std::string>{};

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(directory)) {
    PLOG_ERROR << "search_files: entry " << entry.path();
    if (std::filesystem::is_regular_file(entry) &&
        std::regex_match(entry.path().filename().string(), regex)) {
      PLOG_ERROR << "search_files: found " << entry.path();
      std::cout << entry.path() << std::endl;
      found_files.push_back(entry.path());
    }
  }
  return found_files;
}

std::string FileUtils::read_file_content(std::string filename) {
  std::ifstream inputFile(filename);
  std::ostringstream oss;

  if (inputFile.is_open()) {
    std::string line;
    while (std::getline(inputFile, line)) {
      oss << line << std::endl;
    }
    inputFile.close();
  } else {
    std::cerr << "Failed to open the file." << std::endl;
  }
  return oss.str();
}

}  // namespace Utils
