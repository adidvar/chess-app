#ifndef AWINDOW_HPP
#define AWINDOW_HPP

#include "evaluate.hpp"

class AspirationWindow {
 public:
  void PushPrev(Score score) { m_prev = score; }
  bool Push(Score score) {
    iteration++;
    if (score >= GetHigh()) {
      delta = Score{delta.Value() * 2};
      return true;
    } else if (score <= GetLow()) {
      delta = Score{delta.Value() * 2};
      return true;
    } else
      return false;
  };
  Score GetLow() const {
    if (iteration >= 4) return Score::Min();
    if (m_prev != Score{})
      return m_prev - delta;
    else
      return Score::Min();
  };
  Score GetHigh() const {
    if (iteration >= 4) return Score::Max();
    if (m_prev != Score{})
      return m_prev + delta;
    else
      return Score::Max();
  };

  void End();

 private:
  Score m_prev{};
  Score delta{50};
  int iteration = 0;
};

#endif
