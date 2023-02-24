#include "board.h"
#include <algorithm>
#include <stdexcept>

bool Board::ExecuteTurn(Turn turn)
{
    if(TestTurn(turn)==false)
        return false;

    Set(turn.to(),{GetFigure(turn.from()),GetColor(turn.from())});
    Set(turn.from(),{Figure::kEmpty,Color::kWhite});
    if(turn.figure() != Figure::kEmpty)
        Set(turn.to(),{turn.figure(),GetColor(turn.from())});

    //rooking
    auto flags = RookingFlags();
    rooking_flags_ = {false,false,false,false};
    if(turn.from() == Position(7,4) && turn.to() == Position(7,2) )
        Swap(Position(7,0),Position(7,3));
    else if(turn.from() == Position(7,4) && turn.to() == Position(7,6) )
        Swap(Position(7,7),Position(7,5));
    else if(turn.from() == Position(0,4) && turn.to() == Position(0,2) )
        Swap(Position(0,0),Position(0,3));
    else if(turn.from() == Position(0,4) && turn.to() == Position(0,6) )
        Swap(Position(0,7),Position(0,5));
    else{
       rooking_flags_ = flags;
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
    }
    //pawn special moves
    if(Test(Figure::kPawn,turn.from())&&abs(turn.from().x()-turn.to().x()) == 2)
        last_pawn_move_ = turn.to();
    else
        last_pawn_move_ = Position();

    turn_counter_++;
    if(TestEmp(turn.to()))
        passive_turn_counter_++;
    else
        passive_turn_counter_ = 0;
    current_player_color_ = current_player_color_ == Color::kWhite ? Color::kBlack : Color::kWhite;
    return true;
}

bool Board::TestTurn(Turn turn) const
{
      auto turns = GenerateTurns();
      auto count = std::count(turns.begin(),turns.end(),turn);

      return count != 0;
}
bool Board::MateTest() const {
    auto turns = GenerateTurns(current_player_color_ == Color::kWhite ? Color::kBlack : Color::kWhite);

    //find king
    Position king_pos;
    for(size_t i = 0 ; i<64;++i){
        auto cell = GetCell(Position(i));
        if(cell.color == current_player_color_ && cell.type == Figure::kKing)
            king_pos = i;
    }

    for(auto turn : turns)
        if(turn.to() == king_pos && !Test(Figure::kKing,turn.from()))
            return true;
    return false;
}
