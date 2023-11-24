#ifndef EVALUATE_HPP
#define EVALUATE_HPP

#include <string>

#include "bitboard.hpp"

class Score {
  using ScoreType = int;

 public:
  Score();
  explicit Score(ScoreType value);

  bool operator<(Score value) const;
  bool operator>(Score value) const;
  bool operator<=(Score value) const;
  bool operator>=(Score value) const;
  bool operator==(Score value) const;
  bool operator!=(Score value) const;

  Score operator-() const;
  Score operator-(const Score value) const;
  Score operator+(const Score value) const;

  static Score Win(int depth);
  static Score Lose(int depth);
  static Score Tie();
  static Score Value(const BitBoard &board, Color color);
  static Score Max();
  static Score Min();
  static ScoreType FigurePrice(Figure figure);
  ScoreType Value() const;

  std::string ToString() const;
  float ToCentiPawns() const;

 private:
  ScoreType value_;
};

#endif
