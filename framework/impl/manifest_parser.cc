#include "org/posgi/framework/impl/manifest_parser.h"

#include <plog/Log.h>

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "org/osgi/framework/bundle_context.h"

namespace posgi {

namespace ManifestHeader {

// TODO(jhi): check if unused constants will be removed by compiler

const char kManifestPath[] = "META-INF/MANIFEST.MF";

const char kManifestVersion[] = "Manifest-Version";
const char kCreatedBy[] = "Created-By";
const char kSignatureVersion[] = "Signature-Version";
const char kClassPath[] = "Class-Path";
const char kMainClass[] = "Main-Class";

const char kImplementationTitle[] = "Implementation-Title";
const char kImplementationVersion[] = "Implementation-Version";
const char kImplementationVendor[] = "Implementation-Vendor";
const char kImplementationVendorId[] = "Implementation-Vendor-Id";
const char kImplementationUrl[] = "Implementation-URL";
const char kSpecificationTitle[] = "Specification-Title";
const char kSpecificationVersion[] = "Specification-Version";
const char kSpecificationVendor[] = "Specification-Vendor";
const char kSealed[] = "Sealed";

}  // namespace ManifestHeader

/**
 * See MANIFEST specification here:
 * https://docs.oracle.com/javase/8/docs/technotes/guides/jar/jar.html#Manifest_Specification
 *
 * Not yet implemented:
 * - per-entry headers, would be flatten to one line
 */
ManifestParser::ManifestParser() {
  PLOG_INFO << "ManifestParser::ManifestParser()";
}

/**
 * A manifest file can be wrapped around, e.g. at 70 characters.
 * The next line is then indented with a space character.
 * We "flatten" the manifest file to concatenate the lines with leading spaces
 * to the previous line. Empty lines will be ignored.
 */
std::string ManifestParser::flatten(std::string manifest) {
  PLOG_INFO << "Input:" << std::endl << manifest;
  PLOG_VERBOSE << "flatten start: ";

  std::istringstream iss(manifest);
  std::ostringstream oss;

  std::string line, last_line = "";
  while (std::getline(iss, line)) {
    PLOG_VERBOSE << "last_line: '" << last_line << "'";
    oss << last_line;
    PLOG_VERBOSE << "oss 1: '" << last_line << "'";
    // Remove LF and CRLF line endings
    //  || line.back() == '\n')
    if (!line.empty() && (line.back() == '\r')) {
      line.pop_back();  // Remove CR character
    }
    PLOG_VERBOSE << "line: '" << line << "'";
    if (line.empty()) {  // continue with next line
      last_line = "";
      continue;
    }
    if (line[0] == ' ') {
      line = line.substr(1);  // Remove leading space character
    } else {
      if (!last_line.empty()) {  // handles first line which is empty at start
        oss << std::endl;
        PLOG_VERBOSE << "oss 2: endl";
      }
    }
    PLOG_VERBOSE << "last_line = line: '" << line << "'";
    last_line = line;
  }
  oss << last_line;  // output remaining last line
  PLOG_VERBOSE << "oss 3: '" << line << "'";
  PLOG_VERBOSE << "flatten end: " << std::endl;

  auto s = oss.str();
  PLOG_INFO << "Output:" << std::endl << s;
  return s;
}

std::map<std::string, std::string> ManifestParser::parse(std::string manifest) {
  PLOG_INFO << "parse manifest:" << std::endl;
  PLOG_VERBOSE << std::endl << manifest;

  auto flat_manifest = flatten(manifest);

  std::istringstream iss(flat_manifest);
  std::map<std::string, std::string> headers = {};

  std::string line;
  while (std::getline(iss, line)) {
    size_t colonPos = line.find(':');
    if (colonPos != std::string::npos) {
      // Extract key and value
      std::string key = line.substr(0, colonPos);
      boost::trim(key);
      std::string value = line.substr(colonPos + 1);
      boost::trim(value);

      if (key.empty()) {
        PLOG_ERROR << "Could not parse '" << line << "': empty key";
      } else if (value.empty()) {
        PLOG_ERROR << "Could not parse '" << line << "': empty value";
      } else {
        // all ok now, insert the key-value pair into the map
        PLOG_DEBUG << "parse: " << key << ": " << value;
        headers[key] = value;
      }
    } else {
      PLOG_ERROR << "Could not parse '" << line << "': no colon found";
    }
  }
  for (const auto& pair : headers) {
    PLOG_VERBOSE << "parse: " << pair.first << ": " << pair.second;
  }
  // just a check to allow fuzz tests to fail
  /*
  if (headers.count("Manifest-Version") > 0) {
    auto iter = headers.find("Manifest-Version");
    if (iter != headers.end()) {
      auto value = iter->second;
      if (value != "1.0") {
        PLOG_FATAL << "Manifest has wrong version";
        throw std::runtime_error("Could not parse manifest: wrong version");
      }
    }
  }
  */
  return headers;
}

}  // end namespace posgi
