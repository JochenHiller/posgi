module;

#include <string>

export module m;

// it does work outside of exported class
export std::string CONST_OK = "Bundle-Activator";

export class Constants {
 public:
  // GCC-13 will crash with
  // internal compiler error: Segmentation fault: 11
  // on this code, where clang does compile.
  /*
  inline static const std::string CONST_CRASH_1 = "Bundle-Activator";
  inline static const std::string CONST_CRASH_2 = "xxxbundleXactivatorlll";
  inline static const std::string CONST_CRASH_3 = "xxxXactivatorlll";
  */
  // these variants do NOT crash gcc
  static constexpr std::string_view CONST_OK_1 = "Bundle-Activator";
  inline static const std::string CONST_OK_2 = "BundleActivator";
  inline static const std::string CONST_OK_3 = "Bundle-Activato";
  inline static const std::string CONST_OK_4 = "undle-Activator";
  inline static const std::string CONST_OK_5 = "xxxXactivatorll";
};
