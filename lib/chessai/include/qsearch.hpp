#ifndef QSEARCH_HPP
#define QSEARCH_HPP

#include <algorithm>
#include <iostream>

#include "bftable.hpp"
#include "bitboard.hpp"
#include "ordering.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

template <typename T>
class QSearch {
 public:
  QSearch(Color color, Statistics &stat) : color_(color), stat_(stat) {}

  T GetValue(const BitBoard &board, T a = T::Min(), T b = T::Max()) {
    auto approx = T::Value(board, color_);
    auto qvalue = qsearch({board, board.Hash(), Turn()}, a, b);
    return qvalue;
  }

  static T Evaluate(BitBoard board, Color color, Statistics &stat) {
    QSearch<T> core(color, stat);
    return core.GetValue(board);
  }

 private:
  T qsearch(const BitBoardTuple &tuple, T alpha, T beta) {
    auto oldalpha = alpha;

    bool founded = false;
    auto hashed = table_.Search(tuple.hash, founded);

    if (founded) {
      if (hashed->type == SearchElement::PV)
        return hashed->value;
      else if (hashed->type == SearchElement::FailHigh && hashed->value > beta)
        return hashed->value;
      else if (hashed->type == SearchElement::FailHigh)
        alpha = std::max(alpha, hashed->value);
      else if (hashed->type == SearchElement::FailLow)
        beta = std::min(beta, hashed->value);
      /*
      else if (hashed->depth >= depthleft && hashed->type == SearchElement::PV)
        return hashed->value;
*/
    }
    stat_.ExtraNode();

    auto stand_pat = T::Value(tuple.board, color_);
    stand_pat = tuple.board.CurrentColor() == color_ ? stand_pat : -stand_pat;

    if (stand_pat >= beta) return stand_pat;

    T big_delta = 1100;  // queen value

    if (stand_pat < alpha - big_delta) return alpha;

    if (alpha < stand_pat) alpha = stand_pat;

    auto moves = tuple.board.GenerateTuplesFast(
        tuple, tuple.board.CurrentColor(), kall,
        tuple.board.GetColorBitBoard(tuple.board.OpponentColor()));

    if (moves.empty()) return stand_pat;

    ReOrderQ(tuple.board, moves, btable_, table_);
    T bestscore = T::Min();
    for (auto &sub : moves) {
      auto score = -qsearch(sub, -beta, -alpha);
      if (score >= beta) {  // beta cutoff
        bestscore = score;
        btable_.Push(sub.turn);
        break;
      }
      if (score > bestscore) {
        bestscore = score;
        if (score > alpha) {
          alpha = score;
          btable_.Push(sub.turn);
        };
      }
    }

    hashed->hasvalue = true;
    hashed->hash = tuple.hash;
    hashed->value = bestscore;

    if (bestscore > oldalpha && bestscore < beta)
      hashed->type = SearchElement::PV;
    else if (bestscore <= oldalpha)
      hashed->type = SearchElement::FailLow;
    else if (bestscore >= beta)
      hashed->type = SearchElement::FailHigh;

    return bestscore;
  }

 private:
  Color color_;
  Statistics &stat_;
  BFTable btable_;
  TTable table_;
};
#endif
