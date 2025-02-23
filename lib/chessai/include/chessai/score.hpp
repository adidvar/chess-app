#pragma once

#include <atomic>
#include <chesscore/bitboard.hpp>
#include <cmath>
#include <format>
#include <limits>

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

  constexpr static Score checkMate(Color color, int depth);

  constexpr static Score tie();
  static Score getStaticValue(const BitBoard &board);
  static Score getSEE(const BitBoard &board, Turn turn);
  std::string toString(int depth) const;

  constexpr static Score max();
  constexpr static Score min();

  [[nodiscard]] constexpr bool isValid() const;
  [[nodiscard]] constexpr bool isCheckMate() const;

  constexpr int getTurnsToCheckMate(int depth) const;

  //[[nodiscard]] constexpr std::string toCentiPawns() const;

  constexpr operator ScoreType();

 private:
  constexpr static ScoreType clamp(ScoreType);

  ScoreType m_value;
};

using atomic_score = std::atomic<int>;

constexpr Score::ScoreType k_max_depth = 100;
constexpr Score::ScoreType k_invalid =
    std::numeric_limits<Score::ScoreType>::min();
constexpr Score::ScoreType k_min = k_invalid + 1;
constexpr Score::ScoreType k_checkmate_0 = k_min + 1;
constexpr Score::ScoreType k_checkmate_max = k_checkmate_0 + k_max_depth;
// mate range [k_checkmate_0; k_checkmate_max];

constexpr Score::Score() : Score(k_invalid) {}

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

constexpr Score Score::checkMate(Color color, int depth) {
  if (color == Color::White)
    return (k_checkmate_max - depth);
  else
    return -(k_checkmate_max - depth);
}

constexpr Score Score::tie() { return {0}; }

inline Score Score::getStaticValue(const BitBoard &board) {
  EvaluatedBitBoard evaluator(board);
  return clamp(evaluator.evaluate());
}

inline Score Score::getSEE(const BitBoard &board, Turn turn) {
  EvaluatedBitBoard evaluator(board);
  return clamp(evaluator.evaluateSEE(turn));
}

inline std::string Score::toString(int depth) const {
  if (m_value > k_checkmate_max && m_value < -k_checkmate_max)
    return std::format("cp {:.2f}", m_value / 126.0);
  else {
    return std::format("mate {}", getTurnsToCheckMate(depth));
  }
}

constexpr Score Score::max() { return -k_min; }

constexpr Score Score::min() { return k_min; }

constexpr bool Score::isValid() const { return *this != k_invalid; }

constexpr bool Score::isCheckMate() const {
  return m_value <= k_checkmate_max || m_value >= -k_checkmate_max;
}

constexpr int Score::getTurnsToCheckMate(int depth) const {
  if (m_value < 0)
    return depth + m_value - k_checkmate_max;
  else
    return depth - m_value - k_checkmate_max;
}

constexpr Score::operator ScoreType() { return m_value; }

constexpr Score::ScoreType Score::clamp(ScoreType value) {
  if (value <= k_checkmate_max) return k_checkmate_max + 1;
  if (value >= -k_checkmate_max) return -k_checkmate_max - 1;
  return value;
}
