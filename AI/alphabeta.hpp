#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include <bitboard.hpp>
#include <statistics.hpp>

#include <iostream>
#include <algorithm>

inline bool Predicate(const BitBoard &board, Turn t1, Turn t2)
{

	const static uint8_t order[] = {
								   0,
                                   1,
                                   30,
                                   30,
                                   50,
                                   90,
								   0 };

	if (board.TestEmp(t1.to()) != board.TestEmp(t2.to()))
		return !board.TestEmp(t1.to()) < !board.TestEmp(t2.to());
	else if (!board.TestEmp(t1.to()) && !board.TestEmp(t2.to()))
		if (order[board.GetFigure(t1.to())] != order[board.GetFigure(t2.to())])
            return order[board.GetFigure(t1.to())] < order[board.GetFigure(t2.to())];
		else
            return order[board.GetFigure(t1.from())] > order[board.GetFigure(t2.from())];
	else
        return order[board.GetFigure(t1.from())] < order[board.GetFigure(t2.from())];
};

#include <iostream>
using namespace std;

inline void ReOrder(const BitBoard& board, std::vector<Turn> &turns, std::vector<BitBoard> &boards){

     for(size_t k = 0; k < boards.size() ; ++k){
         bool changed = false;
         for(size_t i = 0; i < boards.size()-1-k ; ++i){
            if( Predicate(board,turns[i],turns[i+1]) )
            {
                std::swap(turns[i],turns[i+1]);
                std::swap(boards[i],boards[i+1]);
                changed = true;
            }
         }
         if(changed == false)
             break;
     }
}

template<typename T>
class AlphaBeta
{
    Color color_;
    Statistics &stat_;

    T alphabeta(const BitBoard& bitboard, int depth, T a, T b)
    {
        stat_.Generation();
        std::vector<BitBoard> nodes;

        nodes = bitboard.GenerateSubBoards(bitboard.CurrentColor());
        auto turns = BitBoard::GenerateTurns(bitboard,nodes,bitboard.CurrentColor());

        bool zero_moves = (nodes.size() == 0);

        if(zero_moves){
            if(!bitboard.MateTest())
                return T::Tie();
            else
                return bitboard.CurrentColor() == color_ ? T::CheckMateLose(depth) : T::CheckMateWin(depth);
        } else if(depth == 0){

            stat_.Approximation();
            auto approx = T::Approximate(bitboard, color_);

            return approx;
        }


        ReOrder(bitboard,turns,nodes);
        if(bitboard.CurrentColor() == color_){
            T value = T::Min();
            for( const auto&node : nodes)
            {
                auto nvalue = alphabeta(node, depth - 1, a, b);
                value = std::max(value, nvalue);
                a = std::max(a,value);
                if(b <= a)
                    break;
            }
            return value;
        } else {
            T value = T::Max();
            for( const auto&node : nodes)
            {
                auto nvalue = alphabeta(node, depth - 1, a, b);
                value = std::min(value, nvalue);
                b = std::min(b,value);
                if(b <= a)
                    break;
            }
            return value;
        }
    }

public:

    AlphaBeta(Color color, Statistics &s):
        color_(color),
        stat_(s)
    {}

    T Evaluate(BitBoard board, int depth)
    {
        auto value = alphabeta(board,depth, T::Min() , T::Max());
        return value;
    }

    static T Evaluate(BitBoard board, Color color, int depth, Statistics &stat ){
        AlphaBeta core(color,stat);
        return core.Evaluate(board,depth);
    }
};

#endif
