#include "computer.hpp"
#include <algorithm>
#include <limits>
#include "Magic/magic.hpp"

auto inf = std::numeric_limits<int>::max()-10;

int Mark(const BitBoard &b, Color color)
{
    int value = 0;
    value += 10*count_1s(b.GetBitBoard(Color::kWhite,Figure::kPawn));
    value -= 10*count_1s(b.GetBitBoard(Color::kBlack,Figure::kPawn));
    value += 30*count_1s(b.GetBitBoard(Color::kWhite,Figure::kKnight));
    value -= 30*count_1s(b.GetBitBoard(Color::kBlack,Figure::kKnight));
    value += 30*count_1s(b.GetBitBoard(Color::kWhite,Figure::kBishop));
    value -= 30*count_1s(b.GetBitBoard(Color::kBlack,Figure::kBishop));
    value += 50*count_1s(b.GetBitBoard(Color::kWhite,Figure::kRook));
    value -= 50*count_1s(b.GetBitBoard(Color::kBlack,Figure::kRook));
    value += 90*count_1s(b.GetBitBoard(Color::kWhite,Figure::kQueen));
    value -= 90*count_1s(b.GetBitBoard(Color::kBlack,Figure::kQueen));

    if(color == Color::kBlack)
       value = -value;

    //value += count_1s(b.GetBitBoard(Color::kWhite,Figure::kEmpty));

    return value;
}

int alphabeta(const BitBoard& bitboard, size_t depth, int a, int b, Color color, bool current){
    if (depth == 0)
        return Mark(bitboard,color);

    auto nodes = bitboard.GenerateSubBoards();

    if(nodes.size() == 0){
        return current ? -inf : inf;
    }

    if(current){
        int value = -inf;
        for(auto node : nodes){
            value = std::max(value,alphabeta(bitboard,depth-1,a,b,color,false));
            a = std::max(a,value);
            if(value > b)
                break;
        }
        return value;
    } else {
        int value = inf;
        for(auto node : nodes){
            value = std::min(value,alphabeta(bitboard,depth-1,a,b,color,true));
            b = std::min(b,value);
            if(value < a)
                break;
        }
        return value;
    }
}
/*
function alphabeta(node, depth, α, β, maximizingPlayer) is
    if depth = 0 or node is a terminal node then
        return the heuristic value of node
    if maximizingPlayer then
        value := −∞
        for each child of node do
            value := max(value, alphabeta(child, depth − 1, α, β, FALSE))
            if value > β then
                break (* β cutoff *)
            α := max(α, value)
        return value
    else
        value := +∞
        for each child of node do
            value := min(value, alphabeta(child, depth − 1, α, β, TRUE))
            if value < α then
                break (* α cutoff *)
            β := min(β, value)
        return value
        */


int minimax(const BitBoard& bitboard, size_t depth,Color color,bool current)
{
    if (depth == 0)
        return Mark(bitboard,color);

    auto nodes = bitboard.GenerateSubBoards();

    if(nodes.size() == 0){
        return current ? -inf : inf;
    }

    if(current){
        int value = -inf;
        for( const auto&node : nodes)
            value = std::max(value, minimax(bitboard, depth-1,color, false));
        return value;
    } else {
        int value = inf;
        for( const auto&node : nodes)
            value = std::min(value, minimax(node, depth - 1,color, true));
        return value;
    }
}
#include <iostream>

BitBoard Computer::GetTurn(BitBoard board)
{
    auto nodes = board.GenerateSubBoards();
    int max_value = Evaluate(nodes.front());
    BitBoard max_b = nodes.front();

    for( const auto&node : nodes)
    {
        int value = Evaluate(node);
        if (value > max_value){
            max_value = value;
            max_b = node;
        }
    }
    return max_b;
}

int Computer::Evaluate(BitBoard board)
{
    //return alphabeta(board,4,-inf,inf,color_,board.CurrentColor() == color_);
    return minimax(board,5,color_,board.CurrentColor() == color_);
}

int Computer::EvaluateA(BitBoard board)
{
    return alphabeta(board,5,-inf,inf,color_,board.CurrentColor() == color_);
    //return minimax(board,4,color_,board.CurrentColor() == color_);
}
