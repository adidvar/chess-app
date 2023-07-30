#ifndef MINIMAX_HPP
#define MINIMAX_HPP

#include <bitboard.hpp>
#include <statistics.hpp>

template<typename T>
class MinMax
{
    Color color_;
    Statistics &stat_;

    T minimax(const BitBoard &bitboard, size_t depth, size_t max_depth) {
      auto nodes = bitboard.GenerateSubBoards(bitboard.CurrentColor());

      bool zero_moves = (nodes.size() == 0);
      bool mate = bitboard.MateTest();

      if (zero_moves && !mate) {
        return T::Tie();
      } else if (zero_moves && mate) {
        return bitboard.CurrentColor() == color_
                   ? T::CheckMateLose(max_depth - depth)
                   : T::CheckMateWin(max_depth - depth);
      } else if (depth == 0) {
        auto approx = T::Approximate(bitboard, color_);

        return approx;
      }

      if (bitboard.CurrentColor() == color_) {
        T value = T::Min();
        for (const auto &node : nodes)
          value = std::max(value, minimax(node, depth - 1, max_depth));
        return value;
      } else {
        T value = T::Max();
        for (const auto &node : nodes)
          value = std::min(value, minimax(node, depth - 1, max_depth));
        return value;
      }
    }

public:

    MinMax(Color color, Statistics &s):
    color_(color),
      stat_(s)
    {}

    T Evaluate(BitBoard board, int depth) {
      auto value = minimax(board, depth, depth);
      return value;
    }

    static T Evaluate(BitBoard board, Color color, int depth, Statistics &stat ){
         MinMax core(color,stat);
         return core.Evaluate(board,depth);
    }
};

#endif
