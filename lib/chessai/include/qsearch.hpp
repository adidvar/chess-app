#ifndef QSEARCH_HPP
#define QSEARCH_HPP

#include <algorithm>
#include <iostream>

#include "bftable.hpp"
#include "bitboard.hpp"
#include "ordering.hpp"
#include "statistics.hpp"

class QSearch {
  using T = Evaluate;

 public:
  QSearch(Color color) : m_color(color) {}

  T GetValue(const BitBoard &board, T a = T::Min(), T b = T::Max()) {
    auto approx = T::Value(board, m_color);
    auto qvalue = qsearch({board, board.Hash(), Turn()}, a, b);
    return qvalue;
  }

 private:
  T qsearch(const BitBoardTuple &tuple, T alpha, T beta) {
    m_stat.ExtraNode();

    auto stand_pat = T::Value(tuple.board, m_color);
    stand_pat = tuple.board.CurrentColor() == m_color ? stand_pat : -stand_pat;

    if (stand_pat >= beta) return stand_pat;

    T big_delta = 1100;  // queen value

    if (stand_pat < alpha - big_delta) return alpha;

    if (alpha < stand_pat) alpha = stand_pat;

    auto moves = tuple.board.GenerateTuplesFast(
        tuple, tuple.board.CurrentColor(), kall,
        tuple.board.GetColorBitBoard(tuple.board.OpponentColor()));

    if (moves.empty()) return stand_pat;

    ReOrderQ(tuple.board, moves, m_btable);
    T bestscore = T::Min();
    Turn bestturn = Turn();
    for (auto &sub : moves) {
      auto score = -qsearch(sub, -beta, -alpha);
      if (score >= beta) {  // beta cutoff
        bestscore = score;
        bestturn = sub.turn;
        m_btable.Push(sub.turn, 0);
        break;
      }
      if (score > bestscore) {
        bestscore = score;
        bestturn = sub.turn;
        if (score > alpha) {
          alpha = score;
          m_btable.Push(sub.turn, 0);
        };
      }
    }

    return bestscore;
  }

 private:
  Color m_color;
  Statistics m_stat;
  BFTable m_btable;
};
#endif
