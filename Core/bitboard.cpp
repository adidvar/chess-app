#include "bitboard.h"
#include <stdexcept>
#include <sstream>
#include <cassert>

const char* BitBoard::kStartPosition_ = u8"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

BitBoard::BitBoard(std::string_view fen)
{
    size_t i = 0;
    size_t position = 0;
    while(i < fen.size() && position != 64 && fen[i]!=' ')
    {
        switch (fen.at(i)) {
        case 'p':
            Set(position,{Figure::kPawn,Color::kBlack});
            position++;
            break;
        case 'n':
            Set(position,{Figure::kKnight,Color::kBlack});
            position++;
            break;
        case 'b':
            Set(position,{Figure::kBishop,Color::kBlack});
            position++;
            break;
        case 'r':
            Set(position,{Figure::kRook,Color::kBlack});
            position++;
            break;
        case 'q':
            Set(position,{Figure::kQueen,Color::kBlack});
            position++;
            break;
        case 'k':
            Set(position,{Figure::kKing,Color::kBlack});
            position++;
            break;
        case 'P':
            Set(position,{Figure::kPawn,Color::kWhite});
            position++;
            break;
        case 'N':
            Set(position,{Figure::kKnight,Color::kWhite});
            position++;
            break;
        case 'B':
            Set(position,{Figure::kBishop,Color::kWhite});
            position++;
            break;
        case 'R':
            Set(position,{Figure::kRook,Color::kWhite});
            position++;
            break;
        case 'Q':
            Set(position,{Figure::kQueen,Color::kWhite});
            position++;
            break;
        case 'K':
            Set(position,{Figure::kKing,Color::kWhite});
            position++;
            break;
        case '1':
            position+=1;
            break;
        case '2':
            position+=2;
            break;
        case '3':
            position+=3;
            break;
        case '4':
            position+=4;
            break;
        case '5':
            position+=5;
            break;
        case '6':
            position+=6;
            break;
        case '7':
            position+=7;
            break;
        case '8':
            position+=8;
            break;
        case '/':
            position = ((position-1)/8)*8+8  ;
            break;
        case ' ':
            break;
        default:
            throw std::runtime_error("fen invalid format [board]");
            break;
        }
        ++i;
    }
    if(position!=64)
        throw std::runtime_error("fen invalid format [board]");

    std::stringstream ss;
    ss << fen.substr(i+1);

    std::string current_move , rooking , pawn;
    size_t static_move, move_counter;
    ss >> current_move >> rooking >> pawn >> static_move >> move_counter;

    if(pawn.size()==2)
        last_pawn_move_ = Position('8'-pawn[1], pawn[0]-'a');

    if(current_move.front() == 'w')
        current_player_color_ = Color::kWhite;
    else if(current_move == "b")
        current_player_color_ = Color::kBlack;
    else
        throw std::runtime_error("fen invalid format [current_move]");
    passive_turn_counter_ = static_move;
    turn_counter_ = move_counter;

    if(current_player_color_ == Color::kWhite)
        last_pawn_move_ = Position(last_pawn_move_.x()+1,last_pawn_move_.y());
    else
        last_pawn_move_ = Position(last_pawn_move_.x()-1,last_pawn_move_.y());

    rooking_flags_ = {false,false,false,false};

    for(char x : rooking)
    {
        switch (x) {
        case 'K':
            rooking_flags_.white_oo = true;
            break;
        case 'Q':
            rooking_flags_.white_ooo = true;
            break;
        case 'k':
            rooking_flags_.black_oo = true;
            break;
        case 'q':
            rooking_flags_.black_ooo = true;
            break;
        case '-':
            break;
        default:
            throw std::runtime_error("fen invalid format [rooking]");
        }
    }
    all_[0] = 0;
    all_[1] = 0;
    for(size_t i = 1 ; i < 7 ; i++){
        all_[0] |= board_[Color::kWhite][i];
        all_[1] |= board_[Color::kBlack][i];
    }
    board_[0][0] = ~(all_[0] | all_[1]);
    board_[1][0] = ~(all_[0] | all_[1]);
}

Color BitBoard::CurrentColor() const noexcept
{
    return current_player_color_;
}

Color BitBoard::OpponentColor() const noexcept
{
    return current_player_color_ == Color::kWhite ? Color::kBlack : Color::kWhite;
}

void BitBoard::Set(Position position, Cell cell) ///< Записує фігуру
{
    for(size_t i = 1 ; i < 7 ; i++)
    {
        board_[0][i] &= ~((uint64_t)1<<position.Value());
        board_[1][i] &= ~((uint64_t)1<<position.Value());
    }

    board_[cell.color][cell.type] |= ((uint64_t)1<<position.Value());

    all_[0] = 0;
    all_[1] = 0;
    for(size_t i = 1 ; i < 7 ; i++){
        all_[0] |= board_[Color::kWhite][i];
        all_[1] |= board_[Color::kBlack][i];
    }
    board_[1][0] = board_[0][0] = ~(all_[0] | all_[1]);
}

void BitBoard::Swap(Position p1, Position p2)
{
    assert(p1.Valid());
    assert(p2.Valid());
    auto cell = GetCell(p1);
    Set(p1,GetCell(p2));
    Set(p2,cell);
}

void BitBoard::UpdateState()
{

}

void BitBoard::SkipMove()
{
    current_player_color_ = OpponentColor();
}
bool BitBoard::Test(Figure figure, Position position) const noexcept
{
    assert(position.Valid());
    return ((board_[Color::kWhite][figure]  | (board_[Color::kBlack][figure])) >> position.Value()) & 1;
}

bool BitBoard::TestColor(Color color, Position position) const noexcept
{
    assert(position.Valid());
    return (all_[color] >> position.Value()) & 1;
}

bool BitBoard::TestEmp(Position position) const noexcept
{
    assert(position.Valid());
    return ( board_[0][0] >> position.Value()) & 1;
}
BitBoard::Cell BitBoard::GetCell(Position position) const noexcept
{
    assert(position.Valid());
    return {GetFigure(position),GetColor(position)};
}

Figure BitBoard::GetFigure(Position position) const noexcept
{
    assert(position.Valid());

    for(size_t i =0 ; i < 7 ; i++){
       if( Test(i,position) )
           return (i);
    }
}

Color BitBoard::GetColor(Position position) const noexcept
{
    assert(position.Valid());
    return TestColor(Color::kBlack,position);
}

/*
BitBoard::operator bool() const noexcept
{
    return TurnGenerate(*this,current_color).size() != 0;
}
*/
