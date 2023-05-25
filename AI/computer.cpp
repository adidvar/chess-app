#include "computer.hpp"
#include <algorithm>
#include <limits>

/*
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

    return value;
}
*/
/*
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



MateAppraiser minimaxab(const BitBoard& bitboard, size_t depth, Color color, MateAppraiser a, MateAppraiser b)
{
    auto nodes = bitboard.GenerateSubBoards();

    bool zero_moves = (nodes.size() == 0);
    bool mate = bitboard.MateTest();

    if(zero_moves && !mate){
        return MateAppraiser::Tie();
    } else if(zero_moves && mate){
        return bitboard.CurrentColor() == color ? MateAppraiser::CheckMateLose() : MateAppraiser::CheckMateWin();
    } else if(depth == 0){
        return MateAppraiser::Approximate(bitboard, color);
    }

    if(bitboard.CurrentColor() == color){
        MateAppraiser value = MateAppraiser::Min();
        for( const auto&node : nodes)
        {
            value = std::max(value, minimaxab(node, depth - 1,color,a ,b));
           // if(value > b)
            //    break;
            a = std::max(a,value);
        }
        return value;
    } else {
        MateAppraiser value = MateAppraiser::Max();
        for( const auto&node : nodes)
        {
            value = std::min(value, minimaxab(node, depth - 1,color,a ,b));
          //  if(value < a)
         //       break;
            b = std::min(b,value);
        }
        return value;
    }
}
/*
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
*/


MateAppraiser EvaluateAB(BitBoard board, Color color , int depth)
{
    return minimaxab(board,depth,color,MateAppraiser::Min(),MateAppraiser::Max());
}
