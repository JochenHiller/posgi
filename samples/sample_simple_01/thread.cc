/**
 * @brief Simple test program to check synchronization between threads.
 */

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;
std::condition_variable cv;
bool isSignaled = false;

// Function executed by the first thread
void waitForSignal() {
  std::unique_lock<std::mutex> lock(mtx);
  while (!isSignaled) {
    cv.wait(lock);
  }
  std::cout << "Thread 1: Signal received!" << std::endl;
}

// Function executed by the second thread
void sendSignal() {
  std::this_thread::sleep_for(std::chrono::seconds(2));
  {
    std::lock_guard<std::mutex> lock(mtx);
    isSignaled = true;
  }
  cv.notify_one();
  std::cout << "Thread 2: Signal sent!" << std::endl;
}

int main() {
  std::thread t1(waitForSignal);
  std::thread t2(sendSignal);

  t1.join();
  t2.join();

  return 0;
}
