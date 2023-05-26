#include "org/posgi/framework/impl/manifest_parser.h"

#include <iostream>
#include <string>

#include "boost/algorithm/string.hpp"
#include "org/osgi/framework/bundle_context.h"
#include "plog/Log.h"

namespace posgi {

ManifestParser::ManifestParser() {
  PLOG_INFO << "ManifestParser::ManifestParser()";
}

std::map<std::string, std::string> ManifestParser::parse(std::string manifest) {
  PLOG_INFO << "ManifestParser::parse()";
  std::map<std::string, std::string> headers;
  // read from string as input string stream
  std::istringstream iss(manifest);
  std::string line;
  while (std::getline(iss, line)) {
    // Split the line by the delimiter ":"
    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(":"));
    // TODO(JochenHiller): what to do with "key: value1:value2"
    if (tokens.size() == 0) {
      PLOG_ERROR << "Could not parse '" << line << "': number of tokens = 0";
      continue;
    } else if (tokens.size() == 1) {
      PLOG_ERROR << "Could not parse '" << line << "': number of tokens = 1";
      continue;
    } else if (tokens.size() == 2) {
      if (tokens[0].empty() || tokens[1].empty()) {
        PLOG_ERROR << "Could not parse '" << line << "': empty key or value";
        continue;
      } else {
        std::string key = tokens.at(0);
        boost::trim_right(key);
        std::string value = tokens.at(1);
        boost::trim(value);
        if (key.find(" ") != std::string::npos) {
          PLOG_ERROR << "Could not parse '" << line << "': key contains spaces";
          continue;
        } else {
          // all ok now, insert the key-value pair into the map
          headers[key] = value;
        }
      }
    }
  }
  return headers;
}

}  // end namespace posgi
