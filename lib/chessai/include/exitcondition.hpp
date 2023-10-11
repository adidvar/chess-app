#ifndef EXITCONDITION_H
#define EXITCONDITION_H

#include <atomic>
#include <future>

struct SearchExitException {};

inline void CheckAndThrow(std::atomic_bool &flag) {
  if (flag.load() == true) throw SearchExitException{};
}

#endif
