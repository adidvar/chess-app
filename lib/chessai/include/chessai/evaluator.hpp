#pragma once

#include <chesscore/bitboard.hpp>

class EvaluatedBitBoard : public BitBoard {
 public:
  using ScoreType = int;

  explicit EvaluatedBitBoard(const BitBoard &board) : BitBoard(board) {}

  static float toCentiPawns(int value);

  ScoreType evaluate() const;

 protected:
  int getMaterial() const;
  int getTables() const;
};
