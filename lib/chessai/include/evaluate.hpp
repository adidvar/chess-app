#ifndef EVALUATE_HPP
#define EVALUATE_HPP

#include <string>

#include "bitboard.hpp"

class Evaluate {
  using ScoreType = int;

 public:
  Evaluate();
  Evaluate(ScoreType value);

  bool operator<(const Evaluate value) const;
  bool operator>(const Evaluate value) const;
  bool operator<=(const Evaluate value) const;
  bool operator>=(const Evaluate value) const;
  bool operator==(const Evaluate value) const;
  bool operator!=(const Evaluate value) const;

  Evaluate operator-() const;
  Evaluate operator-(const Evaluate value) const;

  static Evaluate Win(int depth);
  static Evaluate Lose(int depth);
  static Evaluate Tie();
  static Evaluate Value(const BitBoard &board, Color color);
  static Evaluate Max();
  static Evaluate Min();
  static ScoreType FigurePrice(Figure figure);
  ScoreType Value() const;

  std::string ToString() const;

 private:
  ScoreType value_;
};

#endif
