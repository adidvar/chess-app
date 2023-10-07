#ifndef EXITCONDITION_H
#define EXITCONDITION_H

#include <atomic>
#include <future>

struct SearchExitException {};

void CheckAndThrow(std::atomic_bool &flag) {
  if (flag.load() == true) throw SearchExitException{};
}

void StopAfter(std::atomic_bool &flag, int msec) {
  auto result = std::async(std::launch::async, [&flag, msec]() {
    std::this_thread::sleep_for(std::chrono::milliseconds{msec});
    flag = 1;
  });
}

#endif
