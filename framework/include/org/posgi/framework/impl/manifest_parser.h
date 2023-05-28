#ifndef POSGI_ORG_POSGI_FRAMEWORK_IMPL_MANIFEST_PARSER_H
#define POSGI_ORG_POSGI_FRAMEWORK_IMPL_MANIFEST_PARSER_H

#include <map>
#include <string>

#include "org/osgi/framework/bundle_context.h"
#include "org/posgi/framework/impl/framework_impl.h"

namespace posgi {

class ManifestParser {
 public:
  ManifestParser();
  std::string flatten(std::string manifest);
  std::map<std::string, std::string> parse(std::string manifest);
};

}  // end namespace posgi

#endif  // POSGI_ORG_POSGI_FRAMEWORK_IMPL_MANIFEST_PARSER_H
