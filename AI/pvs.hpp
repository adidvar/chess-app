#ifndef PVS_HPP
#define PVS_HPP

#include <algorithm>
#include <iostream>

#include "bitboard.hpp"
#include "qsearch.hpp"
#include "statistics.hpp"
#include "transpositiontable.hpp"

template <typename T>
inline void ReOrderPVS(const BitBoard &board, T begin, T end) {
  std::sort(begin, end, [&](const BitBoardTuple &t1, const BitBoardTuple &t2) {
    const static uint8_t order[] = {0, 1, 30, 30, 50, 90, 0};

    if (board.TestEmp(t1.turn.to()) != board.TestEmp(t2.turn.to()))
      return !board.TestEmp(t1.turn.to()) < !board.TestEmp(t2.turn.to());
    else if (!board.TestEmp(t1.turn.to()) && !board.TestEmp(t2.turn.to()))
      if (order[board.GetFigure(t1.turn.to())] !=
          order[board.GetFigure(t2.turn.to())])
        return order[board.GetFigure(t1.turn.to())] <
               order[board.GetFigure(t2.turn.to())];
      else
        return order[board.GetFigure(t1.turn.from())] <
               order[board.GetFigure(t2.turn.from())];
    else
      return order[board.GetFigure(t1.turn.from())] <
             order[board.GetFigure(t2.turn.from())];
  });
}

template <typename T>
class Pvs {
  std::pair<T, Turn> pvs(const BitBoardTuple &tuple, const int depth,
                         const int max_depth, T a, T b) {
    /*
    auto hashed = table_.Search(tuple.hash);
    if (hashed.has_value() && hashed->hash == tuple.hash &&
        hashed->depth >= depth) {
      if (hashed->value.first > hashed->a && hashed->value.first < hashed->b)
        return hashed->value;
    }

    if (hashed.has_value() && hashed->hash == tuple.hash &&
        hashed->depth == depth) {
      // if (hashed->value.first <= hashed->a) b = std::min(b, hashed->a);
      //  else if (hashed->value.first >= hashed->b)
      //   a = std::min(a, hashed->b);

      // if (b <= a) return hashed->value;
    }
*/

    /*
    if (depth == 1) {
      auto stand_pat = T::Approximate(tuple.board, color_);
      if (stand_pat >= b) return {b, Turn()};

      int big_delta = 900;  // queen value

      if (stand_pat < a - big_delta) {
        return {a, Turn()};
      }
    }
*/

    auto nodes =
        tuple.board.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (nodes.size() == 0)
      if (!tuple.board.MateTest())
        return {T::Tie(), Turn()};
      else
        return {tuple.board.CurrentColor() == color_
                    ? T::CheckMateLose(max_depth - depth)
                    : T::CheckMateWin(max_depth - depth),
                Turn()};
    else if (depth == 0) {
      return {qsearch_.GetValue(tuple.board, a, b), Turn()};
      // return {T::Approximate(tuple.board, color_), Turn()};
    }
    stat_.MainNode();

    /*
    if (hashed.has_value() && tuple.board.OpponentColor() == color_) {
      auto best_hashed = hashed.value().value.second;

      for (size_t i = 0; i < nodes.size(); ++i)
        if (nodes[i].turn == best_hashed) {
          std::swap(nodes[i], nodes[0]);
          break;
        }

      ReOrder(tuple.board, nodes.rbegin(), nodes.rend() - 1);
    } else
*/
    ReOrder(tuple.board, nodes.rbegin(), nodes.rend());

    bool SearchPV = true;
    T score = T::Invalid();
    for (auto &node : nodes) {
      if (SearchPV) {
        score = -pvs(node, depth - 1, max_depth, -b, -a).first;
      } else {
        score = -pvs(node, depth - 1, max_depth, -a - 1, -a).first;
        if (score > a)  // in fail-soft ... && score < beta ) is common
          score = -pvs(node, depth - 1, max_depth, -b, -a).first;  // re-search
      }
      if (score >= b) return {b, Turn()};  // fail-hard beta-cutoff
      if (score > a) {
        a = score;         // alpha acts like max in MiniMax
        SearchPV = false;  // *1)
      }
    }
    return {a, Turn()};  // fail-hard

    /*
  if (!hashed.has_value() || (hashed->hash != tuple.hash) ||
      (hashed->hash == tuple.hash && hashed->depth < depth)) {
    SearchElement elem;
    elem.hash = tuple.hash;
    elem.value = {value, best_turn};
    elem.depth = depth;
    elem.a = a;
    elem.b = b;
    table_.Write(tuple.hash, elem);
  }

  return {value, best_turn};
*/
  }

 public:
  Pvs(Color color, Statistics &stat, TransPositionTable &table)
      : qsearch_(color, stat), color_(color), stat_(stat), table_(table) {}

  T GetValue(const BitBoard &board, int depth, T a = T::Min(), T b = T::Max()) {
    return pvs({board, board.Hash(), Turn()}, depth, depth, a, b).first;
  }

  Turn GetBestTurn(const BitBoard &board, int depth) {
    return pvs({board, board.Hash(), Turn()}, depth, depth, T::Min(), T::Max())
        .second;
  }

  static T Evaluate(BitBoard board, Color color, int depth, Statistics &stat) {
    TransPositionTable table;
    Pvs<T> core(color, stat, table);
    return core.GetValue(board, depth);
  }

 private:
  Color color_;
  Statistics &stat_;
  TransPositionTable &table_;
  QSearch<T> qsearch_;
};

#endif
