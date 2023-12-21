#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "bitboard.hpp"
#include "score.hpp"
#include "searchsettings.hpp"

class Evaluator {
 public:
  Evaluator(const BitBoard &board, Color color, const GameStage &settings);

  Score::ProcessType Evaluate();

  static Score::ProcessType GetFigurePrice(Figure figure);

 private:
  const BitBoard &m_board;
  const bitboard_t white_mask;
  const bitboard_t black_mask;
  const GameStage &m_settings;
  const Color m_color;
};

#endif
