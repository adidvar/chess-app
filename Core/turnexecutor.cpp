#include "board.hpp"
#include <algorithm>
#include <stdexcept>
#include <cmath>

void Board::UnsafeExecuteTurn(Turn turn)
{
    if(last_pawn_move_.Valid() &&
            Test(Figure::kPawn,turn.from()) &&
            TestEmp(turn.to()) &&
            abs(last_pawn_move_.x() - turn.to().x())==1 &&
            abs(last_pawn_move_.y() - turn.from().y())==1 &&
            last_pawn_move_.y() == turn.to().y())
    {
        Set(last_pawn_move_,Cell{Figure::kEmpty,Color::kWhite});
    }
    //pawn special moves
    if(Test(Figure::kPawn,turn.from())&&abs(turn.from().x()-turn.to().x()) == 2)
        last_pawn_move_ = turn.to();
    else
        last_pawn_move_ = Position();
    //turn
    Set(turn.to(),{GetFigure(turn.from()),GetColor(turn.from())});
    if(turn.figure() != Figure::kEmpty)
        Set(turn.to(),{turn.figure(),GetColor(turn.from())});
    Set(turn.from(),{Figure::kEmpty,Color::kWhite});
    //rooking
    if(Test(Figure::kKing,turn.to())){
        if(turn.from() == Position(7,4) && turn.to() == Position(7,2) ){
            Swap(Position(7,0),Position(7,3));
        }
        else if(turn.from() == Position(7,4) && turn.to() == Position(7,6) ){
            Swap(Position(7,7),Position(7,5));
        }
        else if(turn.from() == Position(0,4) && turn.to() == Position(0,2) ){
            Swap(Position(0,0),Position(0,3));
        }
        else if(turn.from() == Position(0,4) && turn.to() == Position(0,6) ){
            Swap(Position(0,7),Position(0,5));
        }
    }
    //rooking disabling
    if(turn.from()== Position(7,0))
        rooking_flags_.white_ooo = false;
    else if( turn.from() == Position(7,7) )
        rooking_flags_.white_oo = false;
    else if(turn.from()== Position(0,0))
        rooking_flags_.black_ooo = false;
    else if( turn.from() == Position(0,7) )
        rooking_flags_.black_oo = false;
    else if( turn.from() == Position(7,4)){
        rooking_flags_.white_oo = false;
        rooking_flags_.white_ooo = false;
    }else if( turn.from() == Position(0,4)){
        rooking_flags_.black_oo = false;
        rooking_flags_.black_ooo = false;
    }else if(turn.to()== Position(7,0))
        rooking_flags_.white_ooo = false;
    else if( turn.to() == Position(7,7) )
        rooking_flags_.white_oo = false;
    else if(turn.to()== Position(0,0))
        rooking_flags_.black_ooo = false;
    else if( turn.to() == Position(0,7) )
        rooking_flags_.black_oo = false;

    turn_counter_++;
    if(TestEmp(turn.to()))
        passive_turn_counter_++;
    else
        passive_turn_counter_ = 0;
    current_player_color_ = OpponentColor();
}

bool Board::ExecuteTurn(Turn turn)
{
    if(TestTurn(turn)==false)
        return false;

    UnsafeExecuteTurn(turn);
    return true;
}

bool Board::TestTurn(Turn turn) const
{
    auto turns = GenerateTurns();
    auto count = std::count(turns.begin(),turns.end(),turn);

    return count != 0;
}
