#ifndef MINIMAX_HPP
#define MINIMAX_HPP

#include <bitboard.hpp>

template<typename T>
class MinMax
{
    Color color_;

    T minimax(const BitBoard& bitboard, size_t depth)
    {
        auto nodes = bitboard.GenerateSubBoards();

        bool zero_moves = (nodes.size() == 0);
        bool mate = bitboard.MateTest();

        if(zero_moves && !mate){
            return T::Tie();
        } else if(zero_moves && mate){
            return bitboard.CurrentColor() == color_ ? T::CheckMateLose() : T::CheckMateWin();
        } else if(depth == 0){
            return T::Approximate(bitboard, color_);
        }

        if(bitboard.CurrentColor() == color_){
            T value = T::Min();
            for( const auto&node : nodes)
                value = std::max(value, minimax(node, depth - 1).Process());
            return value;
        } else {
            T value = T::Max();
            for( const auto&node : nodes)
                value = std::min(value, minimax(node, depth - 1).Process());
            return value;
        }
    }

public:

    MinMax(Color color):
    color_(color){}

    T Evaluate(BitBoard board, int depth)
    {
        return minimax(board,depth);
    }

};

#endif
