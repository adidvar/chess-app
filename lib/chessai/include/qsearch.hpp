#ifndef QSEARCH_HPP
#define QSEARCH_HPP

#include <algorithm>
#include <iostream>

#include "bftable.hpp"
#include "bitboard.hpp"
#include "ordering.hpp"
#include "search.hpp"
#include "statistics.hpp"

class QSearch : public Search {
  using T = Score;

 public:
  QSearch(Color color) : Search(color) {}

  T QuiescenceSearch(const BitBoardTuple &tuple, T a = T::Min(),
                     T b = T::Max()) {
    auto qvalue = qsearch(tuple, a, b);
    return qvalue;
  }

 private:
  T qsearch(const BitBoardTuple &tuple, T alpha, T beta) {
    m_stat.ExtraNode();

    CheckStopFlag();

    auto stand_pat = T::Value(tuple.board, m_color);
    stand_pat = tuple.board.CurrentColor() == m_color ? stand_pat : -stand_pat;

    if (stand_pat >= beta) return stand_pat;

    if (alpha < stand_pat) alpha = stand_pat;

    auto moves = BitBoard::GenerateTuplesFast(
        tuple, tuple.board.CurrentColor(), kall,
        tuple.board.GetColorBitBoard(tuple.board.OpponentColor()));

    if (moves.empty()) return stand_pat;

    ReOrderQ(tuple.board, moves);
    T bestscore = T::Min();
    Turn bestturn = Turn();
    for (auto &sub : moves) {
      auto score = -qsearch(sub, -beta, -alpha);
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
