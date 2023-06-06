/**
 * @brief Simple program to check core dumps.
 */

#include <iostream>

int main() {
  std::cout << "Create core dump!\n";

  std::string s = "Hello, world!";
  auto p = &s;
  std::cout << *p << std::endl;

  // TODO(jhi): Trace/BPT trap: 5 what does this mean?
  // on macOS: see /Library/Logs/DiagnosticReports
  // ls -alt /Library/Logs/DiagnosticReports/ | head
  p = nullptr;
  std::cout << *p << std::endl;

  return 0;
}
