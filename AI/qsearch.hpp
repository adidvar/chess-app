#ifndef QSEARCH_HPP
#define QSEARCH_HPP

#include <algorithm>
#include <bitboard.hpp>
#include <iostream>
#include <statistics.hpp>

template <typename T>
inline void ReOrderQ(const BitBoard &board, T begin, T end) {
  std::sort(begin, end, [&](const BitBoardTuple &t1, const BitBoardTuple &t2) {
    const static uint8_t order[] = {0, 10, 30, 30, 50, 90, 0};

    if (order[board.GetFigure(t1.turn.to())] !=
        order[board.GetFigure(t2.turn.to())])
      return order[board.GetFigure(t1.turn.to())] <
             order[board.GetFigure(t2.turn.to())];
    else
      return order[board.GetFigure(t1.turn.from())] <
             order[board.GetFigure(t2.turn.from())];
  });
}

template <typename T>
class QSearch {
 public:
  QSearch(Color color, Statistics &stat) : color_(color), stat_(stat) {}

  T GetValue(const BitBoard &board, T a = T::Min(), T b = T::Max()) {
    auto approx = T::Approximate(board, color_);
    auto qvalue = qsearch({board, board.Hash(), Turn()}, a, b);
    return qvalue;
  }

  static T Evaluate(BitBoard board, Color color, Statistics &stat) {
    QSearch<T> core(color, stat);
    return core.GetValue(board);
  }

 private:
  T qsearch(const BitBoardTuple &tuple, T a, T b) {
    stat_.ExtraNode();

    auto stand_pat = T::Approximate(tuple.board, color_);
    if (stand_pat >= b) return b;

    int big_delta = 1100;  // queen value

    if (stand_pat < a - big_delta) return a;

    if (a < stand_pat) a = stand_pat;

    auto nodes = tuple.board.GenerateTuplesFast(
        tuple, tuple.board.CurrentColor(), kall,
        tuple.board.GetColorBitBoard(tuple.board.OpponentColor()));

    if (nodes.size() == 0) return stand_pat;

    ReOrderQ(tuple.board, nodes.rbegin(), nodes.rend());

    T value = T::Invalid();

    if (tuple.board.CurrentColor() == color_) {
      value = T::Min();
      for (auto &node : nodes) {
        auto nvalue = qsearch(node, a, b);
        value = std::max(nvalue, value);
        a = std::max(a, value);
        if (b <= a) break;
      }
    } else {
      value = T::Max();
      for (auto &node : nodes) {
        auto nvalue = qsearch(node, a, b);
        value = std::min(nvalue, value);
        b = std::min(b, value);
        if (b <= a) break;
      }
    }

    return value;
  }

 private:
  Color color_;
  Statistics &stat_;
};

#endif
