#include "computer.hpp"
#include <algorithm>
#include <limits>

auto inf = std::numeric_limits<int>::max()-10;

int mark(const BitBoard&board){
    int values[2][7]{
        {0,1,3,3,5,9,0},
        {0,-1,-3,-3,-5,-9,0}
    };
    int mark = 0;
    for(size_t i = 0 ; i< 64 ; ++i){
        mark += values[board.GetColor(i)][board.GetFigure(i)];
    }
    return mark;
}

int alphabeta(const BitBoard& bitboard, size_t depth, int a, int b, bool current){
    auto nodes = bitboard.GenerateSubBoards();
    if (depth == 0 || nodes.size() == 0)
        return mark(bitboard);
    if(current){
        int value = -inf;
        for( const auto&node : nodes)
        {
            value = std::max<int>(value, alphabeta(bitboard, depth-1, a, b, false));
            if( value > b )
                break;
            a = std::max<int>(a, value);
        }
        return value;
    } else {
        int value = inf;
        for( const auto&node : nodes){
            value = std::min<int>(value, alphabeta(node, depth - 1, a, b, true));
            if(value < a)
                break;
            b = std::min(b, value);
        }
        return value;
    }
}

float Computer::Evaluate(BitBoard board){
    return alphabeta(board,6,-inf,inf,false);
}

/*
Turn Computer::GetTurn(BitBoard board){

}
*/
