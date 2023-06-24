#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include "transpositiontable.hpp"
#include <bitboard.hpp>
#include <statistics.hpp>

#include <iostream>
#include <algorithm>


#include <iostream>
using namespace std;

template<typename T>
inline void ReOrder(const BitBoard& board, T begin , T end){

    std::sort(begin,end,[&](const BitBoardTuple &t1, const BitBoardTuple &t2){

    const static uint8_t order[] = {
                                   0,
                                   1,
                                   30,
                                   30,
                                   50,
                                   90,
                                   0 };

    if (board.TestEmp(t1.turn.to()) != board.TestEmp(t2.turn.to()))
        return !board.TestEmp(t1.turn.to()) < !board.TestEmp(t2.turn.to());
    else if (!board.TestEmp(t1.turn.to()) && !board.TestEmp(t2.turn.to()))
        if (order[board.GetFigure(t1.turn.to())] != order[board.GetFigure(t2.turn.to())])
            return order[board.GetFigure(t1.turn.to())] < order[board.GetFigure(t2.turn.to())];
        else
            return order[board.GetFigure(t1.turn.from())] < order[board.GetFigure(t2.turn.from())];
    else
        return order[board.GetFigure(t1.turn.from())] < order[board.GetFigure(t2.turn.from())];

});
}

template<typename T>
class AlphaBeta
{
    std::pair<T,Turn> alphabeta(const BitBoard& bitboard , const bitboard_hash_t hash, const int depth, T a, T b)
    {
        /*
        auto hashed = table_.Find(hash,bitboard);
        if(hashed.has_value() && hashed.value().depth >= depth){
            return {hashed.value().value,hashed.value().best_turn};
        }
        */

        stat_.Generation();
        auto nodes = bitboard.GenerateTuplesFast(bitboard.CurrentColor());

        if(nodes.size() == 0)
            if(!bitboard.MateTest())
                return {T::Tie(),Turn()};
            else
                return {bitboard.CurrentColor() == color_ ? T::CheckMateLose(depth) : T::CheckMateWin(depth),Turn()};
        else if(depth == 0)
            return {T::Approximate(bitboard, color_),Turn()};
/*
        if(hashed.has_value()){
            auto best_hashed = hashed.value().best_turn;

            for(size_t i = 0 ; i < nodes.size() ; ++i)
                if(nodes[i].turn == best_hashed){
                    std::swap(nodes[i],nodes[0]);
                    break;
                }

            ReOrder(bitboard,nodes.rbegin(),nodes.rend()-1);
        }
        else
        */

        ReOrder(bitboard,nodes.rbegin(),nodes.rend());

        T value = T::Invalid();
        Turn best_turn;

        if(bitboard.CurrentColor() == color_){
            value = T::Min();
            for( auto & node : nodes)
            {
                auto nvalue = alphabeta(node.board, node.hash, depth - 1, a, b).first;
                if(nvalue > value){
                    value = nvalue;
                    best_turn = node.turn;
                }

                a = std::max(a,value);
                if(b <= a)
                    break;
            }
        } else {
            value = T::Max();
            for( auto & node : nodes)
            {
                auto nvalue = alphabeta(node.board, node.hash, depth - 1, a, b).first;
                if(nvalue < value){
                    value = nvalue;
                    best_turn = node.turn;
                }

                b = std::min(b,value);
                if(b <= a)
                    break;
            }
        }

        /*
        TransPositionTable::Element element;
        element.best_turn = best_turn;
        element.board = bitboard;
        element.depth = depth;
        element.value = value;

        table_.Write(element,hash);
        */

        return {value,best_turn};
    }
public:
    AlphaBeta(Color color, Statistics &stat, TransPositionTable &table):
        color_(color),
        stat_(stat),
        table_(table)
    {
    }

    T GetValue(const BitBoard &board, int depth){
        return alphabeta(board,board.Hash(),depth,T::Min(),T::Max()).first;
    }

    Turn GetBestTurn(const BitBoard &board, int depth){
        return alphabeta(board,board.Hash(),depth,T::Min(),T::Max()).second;
    }

    static T Evaluate(BitBoard board, Color color, int depth, Statistics &stat ){
        TransPositionTable table;
         AlphaBeta<T> core(color,stat,table);
         return core.GetValue(board,depth);
    }
private:

    Color color_;
    Statistics &stat_;
    TransPositionTable &table_;
};

#endif
