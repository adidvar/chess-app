#include "evaluator.hpp"

#include "magic.hpp"

struct S {
  S() : mg(0), eg(0) {}
  S(int m) : mg(m), eg(m) {}
  S(int m, int e) : mg(m), eg(e) {}

  int mg;
  int eg;

  int operator()(const GameStage &settings) const { return mg; };
};

Evaluator::Evaluator(const BitBoard &board, Color color,
                     const GameStage &settings)
    : m_board(board),
      m_color(color),
      m_settings(settings),
      white_mask(board.AttackMask(Color::kWhite)),
      black_mask(board.AttackMask(Color::kBlack)) {}

Score::ProcessType Evaluator::Evaluate() {
  return (m_color == Color::kWhite ? 1 : -1) *
         (GetMaterial() + GetTables() + GetPawnStructure() + GetMobilityCS());
}

S kfigure_prices[]{S(),         S(126, 208),   S(781, 854),
                   S(825, 915), S(1276, 1380), S(2538, 2682)};

Score::ProcessType Evaluator::GetFigurePrice(Figure figure) {
  return kfigure_prices[figure].mg;
}

int Evaluator::GetMaterial() {
  int value = 0;

  int white_count[6];
  int black_count[6];

  for (int figure = Figure::kKnight; figure < Figure::kKing; ++figure) {
    white_count[figure] = count_1s(m_board.GetBitBoard(Color::kWhite, figure));
    black_count[figure] = count_1s(m_board.GetBitBoard(Color::kBlack, figure));

    value += kfigure_prices[figure](m_settings) *
             (white_count[figure] - black_count[figure]);
  }
  white_count[Figure::kEmpty] =
      count_1s(m_board.GetColorBitBoard(Color::kWhite));
  black_count[Figure::kEmpty] =
      count_1s(m_board.GetColorBitBoard(Color::kBlack));

  return value;
}

int Evaluator::GetTables() {
  int value = 0;

  return value;
}

int Evaluator::GetPawnStructure() {
  int value = 0;

  int line_white[8]{0};
  int file_white[8]{0};
  int line_black[8]{0};
  int file_black[8]{0};

  auto white_pawns = m_board.GetBitBoard(Color::kWhite, Figure ::kPawn);
  auto black_pawns = m_board.GetBitBoard(Color::kBlack, Figure ::kPawn);

  for (int i = 0; i < 8; i++) {
    line_white[i] = count_1s(lines[i] & white_pawns);
    line_black[i] = count_1s(lines[i] & black_pawns);
    file_white[i] = count_1s(rows[i] & white_pawns);
    file_black[i] = count_1s(rows[i] & black_pawns);
  }

  for (int i = 0; i < 8; i++) {
    if (file_white[i] > 2) value += S(-120)(m_settings) * file_white[i];
    if (file_black[i] > 2) value -= S(-120)(m_settings) * file_black[i];
  }

  return value;
}

int Evaluator::GetMobilityCS() {
  int value = 0;

  // mobility
  value += S(3)(m_settings) *
           (count_1s(white_mask & ~m_board.GetColorBitBoard(Color::kWhite)) -
            count_1s(black_mask & ~m_board.GetColorBitBoard(Color::kBlack)));

  // connectivity
  value += S(13)(m_settings) *
           (count_1s(white_mask & m_board.GetColorBitBoard(Color::kWhite)) -
            count_1s(black_mask & m_board.GetColorBitBoard(Color::kBlack)));

  // space
  value += S(2)(m_settings) * (count_1s(white_mask & ~black_mask) -
                               count_1s(black_mask & ~white_mask));

  return value;
}
