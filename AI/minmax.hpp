#ifndef MINIMAX_HPP
#define MINIMAX_HPP

#include <bitboard.hpp>
#include <statistics.hpp>

template<typename T, typename S = NoStatistics>
class MinMax
{
    Color color_;
    S &stat_;

    T minimax(const BitBoard& bitboard, size_t depth)
    {
        stat_.NewNodeEvent();
        stat_.GenerationTimerBegin();
        auto nodes = bitboard.GenerateSubBoards(bitboard.CurrentColor());
        stat_.GenerationTimerEnd();

        bool zero_moves = (nodes.size() == 0);
        bool mate = bitboard.MateTest();

        if(zero_moves && !mate){
            return T::Tie();
        } else if(zero_moves && mate){
            return bitboard.CurrentColor() == color_ ? T::CheckMateLose(depth) : T::CheckMateWin(depth);
        } else if(depth == 0){

            stat_.NewApproximationEvent();
            stat_.ApproximationTimerBegin();
            auto approx = T::Approximate(bitboard, color_);
            stat_.ApproximationTimerEnd();

            return approx;
        }

        if(bitboard.CurrentColor() == color_){
            T value = T::Min();
            for( const auto&node : nodes)
                value = std::max(value, minimax(node, depth - 1));
            return value;
        } else {
            T value = T::Max();
            for( const auto&node : nodes)
                value = std::min(value, minimax(node, depth - 1));
            return value;
        }
    }

public:

    MinMax(Color color, S &s):
    color_(color),
      stat_(s)
    {}

    T Evaluate(BitBoard board, int depth)
    {
        stat_.Clear();
        stat_.AllTimerBegin();
        auto value = minimax(board,depth);
        stat_.AllTimerEnd();
        return value;
    }

    static T Evaluate(BitBoard board, Color color, int depth, S &stat ){
         MinMax core(color,stat);
         return core.Evaluate(board,depth);
    }
};

#endif
