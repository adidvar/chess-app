#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <variant>

#include "figures.hpp"

class SearchManager {
 public:
  SearchManager() = default;

  void SetTime(float w_time, float b_time, float w_increment,
               float b_increment) {
    m_timer = ClassicTimer{w_time, b_time, w_increment, b_increment};
  }
  void SetTime(float time) { m_timer = ConstTimer{time}; }
  void SetTime() { m_timer = InfiniteTimer{}; }

  float GetTime(Color color) {
    if (const auto* pval = std::get_if<ClassicTimer>(&m_timer)) {
      /*
       * TO DO
      if (color == Color::kWhite) {
      } else {
      }
*/
    } else if (const auto* pval = std::get_if<ConstTimer>(&m_timer)) {
      return pval->time;
    } else if (const auto* pval = std::get_if<InfiniteTimer>(&m_timer)) {
      return std::numeric_limits<float>::min();
    }
  }

 private:
  struct ClassicTimer {
    float w_time;
    float b_time;
    float w_increment;
    float b_increment;
  };
  struct ConstTimer {
    float time;
  };
  struct InfiniteTimer {};

  std::variant<ClassicTimer, ConstTimer, InfiniteTimer> m_timer =
      InfiniteTimer{};
};

#endif
