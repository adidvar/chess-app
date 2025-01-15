#pragma once

#include <string>

#include <chesscore/bitboard.hpp>

#include "searchsettings.hpp"

class Score {
 public:
  using ScoreType = int;
  using ProcessType = long int;

  Score();
  [[deprecated]] Score(ProcessType type);

  bool operator<(Score value) const;
  bool operator>(Score value) const;
  bool operator<=(Score value) const;
  bool operator>=(Score value) const;
  bool operator==(Score value) const;
  bool operator!=(Score value) const;

  Score operator-() const;
  Score operator-(Score value) const;
  Score operator+(Score value) const;
  Score operator*(int value) const;

  static Score CheckMate(int depthleft, int depthmax);
  static Score Tie();
  static Score GetStaticValue(const BitBoard &board, Color color,
                              unsigned stage);

  static Score Max();
  static Score Min();

  [[nodiscard]] bool IsValid() const;
  [[nodiscard]] bool IsCheckMate() const;

  int GetTurnsToCheckMate() const;

  static Score GetFigureScore(Figure figure);

  [[nodiscard]] std::string ToCentiPawns() const;

  operator ProcessType();

 private:
  static ScoreType Clamp(ProcessType);

  ScoreType m_value;
};
