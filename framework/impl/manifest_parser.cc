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
    if (tokens.size() != 2) {
      PLOG_ERROR << "Could not parse '" << line << "'";
      continue;
    } else {
      std::string key = tokens.at(0);
      boost::trim(key);
      std::string value = tokens.at(1);
      boost::trim(value);
      // Insert the key-value pair into the map
      headers[key] = value;
    }
  }
  return headers;
}

}  // end namespace posgi
