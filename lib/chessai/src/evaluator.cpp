#include "evaluator.hpp"

#include "magic.hpp"

struct S {
  S(int m, int e) : mg(m), eg(e) {}

  int mg;
  int eg;
};

Evaluator::Evaluator(const BitBoard &board, Color color,
                     const GameStage &settings)
    : m_board(board),
      m_color(color),
      m_settings(settings),
      white_mask(board.AttackMask(Color::kWhite)),
      black_mask(board.AttackMask(Color::kBlack)) {}

Score::ProcessType Evaluator::Evaluate() { return 0; }

Score::ProcessType Evaluator::GetFigurePrice(Figure figure) {
  return (m_color == Color::kWhite ? 1 : -1) *
         (GetMaterial() + GetTables() + GetPawnStructure() + GetMobilityCS());
}

int Evaluator::GetMaterial() {}

int Evaluator::GetTables() {}

int Evaluator::GetPawnStructure() {}

int Evaluator::GetMobilityCS() {}
