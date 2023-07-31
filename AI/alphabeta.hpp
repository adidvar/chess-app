#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include <algorithm>
#include <iostream>

#include "bitboard.hpp"
#include "ordering.hpp"
#include "qsearch.hpp"
#include "statistics.hpp"

template <typename T>
class AlphaBeta {
  std::pair<T, Turn> alphabeta(const BitBoardTuple &tuple, const int depth,
                               const int max_depth, T a, T b) {
    auto nodes =
        tuple.board.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (nodes.size() == 0)
      if (!tuple.board.MateTest())
        return {T::Tie(), Turn()};
      else
        return {tuple.board.CurrentColor() == color_
                    ? T::Lose(max_depth - depth)
                    : T::Win(max_depth - depth),
                Turn()};
    else if (depth == 0) {
      return {qsearch_.GetValue(tuple.board, a, b), Turn()};
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

    T value;
    Turn best_turn;

    if (tuple.board.CurrentColor() == color_) {
      value = T::Min();
      for (auto &node : nodes) {
        auto nvalue = alphabeta(node, depth - 1, max_depth, a, b).first;
        if (nvalue > value) {
          value = nvalue;
          best_turn = node.turn;
        }

        a = std::max(a, value);
        if (b <= a) break;
      }
    } else {
      value = T::Max();
      for (auto &node : nodes) {
        auto nvalue = alphabeta(node, depth - 1, max_depth, a, b).first;
        if (nvalue < value) {
          value = nvalue;
          best_turn = node.turn;
        }

        b = std::min(b, value);
        if (b <= a) break;
      }
    }
    return {value, best_turn};
  }

 public:
  AlphaBeta(Color color, Statistics &stat)
      : qsearch_(color, stat), color_(color), stat_(stat) {}

  T GetValue(const BitBoard &board, int depth, T a = T::Min(), T b = T::Max()) {
    return alphabeta({board, board.Hash(), Turn()}, depth, depth, a, b).first;
  }

  Turn GetBestTurn(const BitBoard &board, int depth) {
    return alphabeta({board, board.Hash(), Turn()}, depth, depth, T::Min(),
                     T::Max())
        .second;
  }

  static T Evaluate(BitBoard board, Color color, int depth, Statistics &stat) {
    AlphaBeta<T> core(color, stat);
    return core.GetValue(board, depth);
  }

 private:
  Color color_;
  Statistics &stat_;
  QSearch<T> qsearch_;
};

#endif
