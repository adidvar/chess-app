#include "board.h"
#include <stdexcept>
#include <sstream>

const char* Board::kStartPosition_ = u8"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

Board::Board(std::string_view fen)
{
    for(size_t i = 0 ; i < 64 ; i++)
        Set(i,{Figure::kEmpty,Color::kWhite});

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

    if(current_move.front() == 'w')
        current_player_color_ = Color::kWhite;
    else if(current_move == "b")
        current_player_color_ = Color::kBlack;
    else
        throw std::runtime_error("fen invalid format [current_move]");
    passive_turn_counter_ = static_move;
    turn_counter_ = move_counter;

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
}

Color Board::CurrentColor() const noexcept
{
    return current_player_color_;
}

Color Board::OpponentColor() const noexcept
{
    return current_player_color_ == Color::kWhite ? Color::kBlack : Color::kWhite;
}

Board::RookingFlags_t Board::RookingFlags() const noexcept
{
    return rooking_flags_;
}

void Board::Set(Position position, Cell cell)
{
    board_[position.Value()] = cell;
}

void Board::Swap(Position p1, Position p2)
{
    std::swap(board_[p1.Value()],board_[p2.Value()]);
}


void Board::UpdateState()
{

}

void Board::SkipMove()
{
    current_player_color_ = OpponentColor();
}

bool Board::Test(Figure figure, Position position) const noexcept
{
    return board_[position.Value()].type == figure;
}

bool Board::TestColor(Color color, Position position) const noexcept
{
    return board_[position.Value()].color == color;
}

bool Board::TestEmp(Position position) const noexcept
{
    return board_[position.Value()].type == Figure::kEmpty;
}

Board::Cell Board::GetCell(Position position) const noexcept
{
    return board_[position.Value()];
}

Figure Board::GetFigure(Position position) const noexcept
{
    return board_[position.Value()].type;
}

Color Board::GetColor(Position position) const noexcept
{
    return board_[position.Value()].color;
}

bool Board::End() const {return state_ != kActiveNow;}

bool Board::Checkmate() const {return state_ == kWhiteCheckmate || state_ == kBlackCheckmate;}

bool Board::WhiteWin() const {return state_ == kBlackCheckmate;}

bool Board::BlackWin() const {return state_ == kWhiteCheckmate;}

bool Board::Tie() const {return state_ == kTie;}

/*
Board::operator bool() const noexcept
{
    return TurnGenerate(*this,current_color).size() != 0;
}
*/
