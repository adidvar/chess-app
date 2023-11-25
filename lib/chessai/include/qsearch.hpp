#ifndef QSEARCH_HPP
#define QSEARCH_HPP

#include "bitboard.hpp"
#include "boarditerator.hpp"
#include "ordering.hpp"
#include "search.hpp"
#include "statistics.hpp"

class QSearch : public Search, private ChessTreeHash {
  using T = Score;

  inline const static Score kBIG_DELTA{
      Score::FigurePrice(Figure::kPawn) +
      Score::FigurePrice(Figure::kQueen)};  // queen value

 public:
  QSearch(Color color) : Search(color) {}

  T QuiescenceSearch(const BitBoardTuple &tuple, T a = T::Min(),
                     T b = T::Max()) {
    auto qvalue = qsearch(tuple, a, b, 0);
    return qvalue;
  }

 private:
  T qsearch(const BitBoardTuple &tuple, T alpha, T beta, int depth) {
    m_stat.ExtraNode();

    CheckStopFlag();

    m_stat.EndNode();
    auto stand_pat = T::Value(tuple.board, m_color);
    stand_pat = tuple.board.CurrentColor() == m_color ? stand_pat : -stand_pat;

    if (stand_pat >= beta) return stand_pat;

    if (stand_pat < alpha - kBIG_DELTA) return alpha;

    if (alpha < stand_pat) alpha = stand_pat;

    BitBoard::GenerateTuplesFast(
        Get(depth), tuple, tuple.board.CurrentColor(), kall,
        tuple.board.GetColorBitBoard(tuple.board.OpponentColor()), false);
    auto &moves = Get(depth);

    if (moves.empty()) return stand_pat;

    ReOrderQ(tuple.board, moves);
    T bestscore = T::Min();
    Turn bestturn = Turn();
    for (auto &sub : moves) {
      auto score = -qsearch(sub, -beta, -alpha, depth + 1);
      if (score >= beta) {  // beta cutoff
        bestscore = score;
        bestturn = sub.turn;
        break;
      }
      if (score > bestscore) {
        bestscore = score;
        bestturn = sub.turn;
        if (score > alpha) {
          alpha = score;
        };
      }
    }

    return bestscore;
  }
};
#endif
