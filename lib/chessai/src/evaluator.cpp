#include "evaluator.hpp"

#include <cmath>

#include "magic.hpp"

struct S {
  S() : mg(0), eg(0) {}
  S(int m) : mg(m), eg(m) {}
  S(int m, int e) : mg(m), eg(e) {}

  int mg;
  int eg;

  int operator()(unsigned) const { return mg; }
};

Evaluator::Evaluator(const BitBoard &board, Color color, unsigned int stage)
    : m_board(board),
      m_color(color),
      m_stage(stage),
      white_mask(board.AttackMask(Color::kWhite) &
                 ~board.GetBitBoard(Color::kWhite, Figure::kQueen)),
      black_mask(board.AttackMask(Color::kBlack) &
                 ~board.GetBitBoard(Color::kBlack, Figure::kQueen)) {}

Score::ProcessType Evaluator::Evaluate() {
  return (m_color == Color::kWhite ? 1 : -1) *
         (GetMaterial() + GetTables() + GetPawnStructure() + GetMobilityCS());
}

float Evaluator::Normalize(int value) {
  return 20.0 / (1.0 + pow(3.0, -value / 1260.0)) - 10;
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

    value += kfigure_prices[figure](m_stage) *
             (white_count[figure] - black_count[figure]);
  }
  white_count[Figure::kEmpty] =
      count_1s(m_board.GetColorBitBoard(Color::kWhite));
  black_count[Figure::kEmpty] =
      count_1s(m_board.GetColorBitBoard(Color::kBlack));

  return value;
}

constexpr static int tables[7][64]{
    {},  // empty
    // pawn
    {0,  0,  0,  0,   0,   0,  0,  0,  50, 50, 50,  50, 50, 50,  50, 50,
     10, 10, 20, 30,  30,  20, 10, 10, 5,  5,  10,  25, 25, 10,  5,  5,
     0,  0,  0,  20,  20,  0,  0,  0,  5,  -5, -10, 0,  0,  -10, -5, 5,
     5,  10, 10, -20, -20, 10, 10, 5,  0,  0,  0,   0,  0,  0,   0,  0},
    // knight
    {
        -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   0,   0,
        0,   -20, -40, -30, 0,   10,  15,  15,  10,  0,   -30, -30, 5,
        15,  20,  20,  15,  5,   -30, -30, 0,   15,  20,  20,  15,  0,
        -30, -30, 5,   10,  15,  15,  10,  5,   -30, -40, -20, 0,   5,
        5,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50,
    },
    // bishop
    {
        -20, -10, -10, -10, -10, -10, -10, -20, -10, 0,   0,   0,   0,
        0,   0,   -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 5,
        5,   10,  10,  5,   5,   -10, -10, 0,   10,  10,  10,  10,  0,
        -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 5,   0,   0,
        0,   0,   5,   -10, -20, -10, -10, -10, -10, -10, -10, -20,
    },
    // rook
    {0,  0, 0, 0, 0, 0, 0, 0,  5,  10, 10, 10, 10, 10, 10, 5,
     -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
     -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
     -5, 0, 0, 0, 0, 0, 0, -5, 0,  0,  0,  5,  5,  0,  0,  0},
    // queen
    {-20, -10, -10, -5,  -5,  -10, -10, -20, -10, 0,   0,   0,  0,
     0,   0,   -10, -10, 0,   5,   5,   5,   5,   0,   -10, -5, 0,
     5,   5,   5,   5,   0,   -5,  0,   0,   5,   5,   5,   5,  0,
     -5,  -10, 5,   5,   5,   5,   5,   0,   -10, -10, 0,   5,  0,
     0,   0,   0,   -10, -20, -10, -10, -5,  -5,  -10, -10, -20},
    // king
    {-30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50,
     -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40,
     -40, -50, -50, -40, -40, -30, -20, -30, -30, -40, -40, -30, -30,
     -20, -10, -20, -20, -20, -20, -20, -20, -10, 20,  20,  0,   0,
     0,   0,   20,  20,  20,  30,  10,  0,   0,   10,  30,  20}};

int Evaluator::GetTables() {
  int value = 0;

  for (int figure = Figure::kPawn; figure <= Figure::kKing; ++figure) {
    BitIterator i = m_board.GetBitBoard(Color::kWhite, figure);
    for (; i.Valid(); ++i) {
      Position const index = log2_64(i.Bit());
      value += tables[figure][index.Value()];
    }
    i = m_board.GetBitBoard(Color::kBlack, figure);
    for (; i.Valid(); ++i) {
      Position const index = log2_64(i.Bit());
      value -= tables[figure][index.Rotate().Value()];
    }
  }
  return 2 * value;
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
    if (file_white[i] > 2) value += S(-80)(m_stage) * file_white[i];
    if (file_black[i] > 2) value -= S(-80)(m_stage) * file_black[i];
  }

  return value;
}

int Evaluator::GetMobilityCS() {
  int value = 0;

  // mobility
  value += S(3)(m_stage) *
           (count_1s(white_mask & ~m_board.GetColorBitBoard(Color::kWhite)) -
            count_1s(black_mask & ~m_board.GetColorBitBoard(Color::kBlack)));

  // connectivity
  value += S(13)(m_stage) *
           (count_1s(white_mask & m_board.GetColorBitBoard(Color::kWhite)) -
            count_1s(black_mask & m_board.GetColorBitBoard(Color::kBlack)));

  // space
  value += S(2)(m_stage) * (count_1s(white_mask & ~black_mask) -
                            count_1s(black_mask & ~white_mask));

  return value;
}
