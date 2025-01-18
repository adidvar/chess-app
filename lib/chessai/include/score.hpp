#pragma once

#include <chesscore/bitboard.hpp>
#include <numeric>

#include "evaluator.hpp"

class Score {
 public:
  using ScoreType = EvaluatedBitBoard::ScoreType;

  constexpr Score();
  constexpr Score(ScoreType type);

  constexpr bool operator<(Score value) const;
  constexpr bool operator>(Score value) const;
  constexpr bool operator<=(Score value) const;
  constexpr bool operator>=(Score value) const;
  constexpr bool operator==(Score value) const;
  constexpr bool operator!=(Score value) const;

  constexpr Score operator-() const;
  constexpr Score operator-(Score value) const;
  constexpr Score operator+(Score value) const;
  constexpr Score operator*(int value) const;

  constexpr static Score checkMate(int depthleft, int depthmax);
  constexpr static Score tie();
  static Score getStaticValue(const BitBoard &board);

  constexpr static Score max();
  constexpr static Score min();

  [[nodiscard]] constexpr bool isValid() const;
  [[nodiscard]] constexpr bool isCheckMate() const;

  constexpr int getTurnsToCheckMate() const;

  static Score getFigureScore(Figure figure);

  //[[nodiscard]] constexpr std::string toCentiPawns() const;

  constexpr operator ScoreType();

 private:
  constexpr static ScoreType clamp(ScoreType);

  ScoreType m_value;
};

constexpr unsigned k_max_depth = 100;
constexpr Score::ScoreType k_invalid = {};
constexpr Score::ScoreType k_min = -((k_invalid - 4) / 2);
constexpr Score::ScoreType k_checkmate_0 = k_min + 1;
constexpr Score::ScoreType k_checkmate_max = k_checkmate_0 + k_max_depth;

constexpr Score::Score()
    : Score(std::numeric_limits<Score::ScoreType>::min()) {}

constexpr Score::Score(ScoreType type) : m_value(type) {}

constexpr bool Score::operator<(Score value) const {
  return m_value < value.m_value;
}

constexpr bool Score::operator>(Score value) const {
  return m_value > value.m_value;
}

constexpr bool Score::operator<=(Score value) const {
  return m_value <= value.m_value;
}

constexpr bool Score::operator>=(Score value) const {
  return m_value >= value.m_value;
}

constexpr bool Score::operator==(Score value) const {
  return m_value == value.m_value;
}

constexpr bool Score::operator!=(Score value) const {
  return m_value != value.m_value;
}

constexpr Score Score::operator-() const { return -this->m_value; }

constexpr Score Score::operator-(Score value) const {
  return clamp(this->m_value - value.m_value);
}

constexpr Score Score::operator+(Score value) const {
  return clamp(this->m_value + value.m_value);
}

constexpr Score Score::operator*(int value) const {
  return clamp(this->m_value * value);
}

constexpr Score Score::checkMate(int depthleft, int depthmax) {
  Score::ScoreType value{k_checkmate_0 + depthmax - depthleft};
  return std::min<Score::ScoreType>(k_checkmate_max, value);
}

constexpr Score Score::tie() { return {0}; }

inline Score Score::getStaticValue(const BitBoard &board) {
  EvaluatedBitBoard evaluator(board);
  return clamp(evaluator.evaluate());
}

constexpr Score Score::max() { return -k_min; }

constexpr Score Score::min() { return k_min; }

constexpr bool Score::isValid() const { return *this != k_invalid; }

constexpr bool Score::isCheckMate() const {
  return m_value <= k_checkmate_max || m_value >= -k_checkmate_max;
}

constexpr int Score::getTurnsToCheckMate() const {
  if (m_value < 0)
    return m_value - k_checkmate_0;
  else
    return -m_value - k_checkmate_0;
}

inline Score Score::getFigureScore(Figure figure) {
  return EvaluatedBitBoard::getFigurePrice(figure);
}

constexpr Score::operator ScoreType() { return m_value; }

constexpr Score::ScoreType Score::clamp(ScoreType value) {
  if (value <= k_checkmate_max) return k_checkmate_max + 1;
  if (value >= -k_checkmate_max) return -k_checkmate_max - 1;
  return value;
}
