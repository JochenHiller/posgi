#include <iomanip>
#include <iostream>

int main() {
  std::cout << "Hello World!" << std::endl;
  std::cout << "[" << std::right << std::setw(2) << 9 << "]" << std::endl;
  return 0;
}
