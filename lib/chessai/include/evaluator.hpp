#pragma once

#include <chesscore/bitboard.hpp>

#include "score.hpp"
#include "searchsettings.hpp"

class Evaluator {
 public:
  Evaluator(const BitBoard &board, Color color, unsigned stage);

  Score::ProcessType Evaluate();

  static float Normalize(int value);
  static Score::ProcessType GetFigurePrice(Figure figure);

 private:
  int GetMaterial();
  int GetTables();
  int GetPawnStructure();
  int GetMobilityCS();

 private:
  const BitBoard &m_board;
  const bitboard_t white_mask;
  const bitboard_t black_mask;
  const unsigned m_stage;
  const Color m_color;
};
