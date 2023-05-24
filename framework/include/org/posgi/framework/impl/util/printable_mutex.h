#ifndef POSGI_ORG_POSGI_FRAMEWORK_IMPL_UTIL_PRINTABLE_MUTEX_H
#define POSGI_ORG_POSGI_FRAMEWORK_IMPL_UTIL_PRINTABLE_MUTEX_H

#include <iostream>
#include <mutex>

namespace posgi {

class PrintableMutex {
 private:
  std::mutex mutex;

 public:
  inline void lock() {
    mutex.lock();
  }

  inline void unlock() {
    mutex.unlock();
  }

  inline bool try_lock() {
    return mutex.try_lock();
  }

  // Custom printing function
  inline void print() {
    std::cout << "Mutex is " << (mutex.try_lock() ? "unlocked" : "locked")
              << std::endl;
    mutex.unlock();
  }
};

}  // namespace posgi

#endif  // POSGI_ORG_POSGI_FRAMEWORK_IMPL_UTIL_PRINTABLE_MUTEX_H