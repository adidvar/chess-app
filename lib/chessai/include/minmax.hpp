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
      if (depth == 0) {
        auto approx = T::Value(bitboard, color_);
        return bitboard.CurrentColor() == color_ ? approx : -approx;
      }

      auto nodes = bitboard.GenerateSubBoards(bitboard.CurrentColor());

      if (nodes.empty()) {
        if (bitboard.Checkmate()) return T::Lose(max_depth - depth);
        return T::Tie();
      }

      T bestscore = T::Min();
      for (auto &sub : nodes) {
        auto score = -minimax(sub, depth - 1, max_depth);
        bestscore = std::max(score, bestscore);
      }
      return bestscore;
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
