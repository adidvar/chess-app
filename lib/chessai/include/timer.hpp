#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

struct TimeExcetion {};

class Timer {
 public:
  Timer(std::chrono::milliseconds milliseconds) : m_delay(milliseconds) {}

  void Init() { m_begin = std::chrono::high_resolution_clock::now(); };

  bool Check() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::high_resolution_clock::now() - m_begin) > m_delay;
  };

  void CheckAndThrow() {
    if (Check()) throw TimeExcetion();
  };

 private:
  std::chrono::high_resolution_clock::time_point m_begin;
  std::chrono::milliseconds m_delay;
};

class NoTimer {
 public:
  NoTimer(){};

  void Init(){};

  bool Check() { return false; };
  void CheckAndThrow(){};
};

#endif
