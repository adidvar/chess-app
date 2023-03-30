#include "board.hpp"
#include <stdexcept>
#include <sstream>
#include <cassert>

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
}

std::string Board::Fen() const
{
    const char *symbols[2] = {
        " PNBRQK",
        " pnbrqk"
    };
    char buffer[128] = {0};
    size_t position = 0;
    size_t bypass_counter = 0;
    for(size_t i = 0 ; i < 64; i++){
        if(i%8==0 && i!=0)
        {
            if(bypass_counter!=0)
            {
                buffer[position] = '0'+bypass_counter;
                position++;
                bypass_counter=0;
            }

            buffer[position] = '/';
            position++;

        }
        if(TestEmp(i))
            bypass_counter++;
        else
        {
            if(bypass_counter!=0)
            {
                buffer[position] = '0'+bypass_counter;
                position++;
                bypass_counter=0;
            }
            buffer[position] = symbols[GetColor(i)][GetFigure(i)];
            position++;
        }
    }

    if(bypass_counter!=0)
    {
        buffer[position] = '0'+bypass_counter;
        position++;
    }

    std::string string(buffer);
    string += ' ';
    string.push_back( CurrentColor() == Color::kWhite ? 'w' : 'b');
    string += ' ';
    auto rooking_flags = RookingFlags();
    if(rooking_flags.white_oo == false &&
            rooking_flags.white_ooo == false &&
            rooking_flags.black_oo == false &&
            rooking_flags.black_ooo == false)
        string.push_back('-');
    else{
    if(rooking_flags.white_oo)
        string +='K';
    if(rooking_flags.white_ooo)
        string +='Q';
    if(rooking_flags.black_oo)
        string +='k';
    if(rooking_flags.black_ooo)
        string +='q';
    }
    string += ' ';

    if(LastPawnMove().Valid())
        string += LastPawnMove().ToString();
    else
        string += '-';
    string += ' ';
    string += std::to_string(PassiveTurnCounter());
    string += ' ';
    string += std::to_string(TurnCounter());


    return string;
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

size_t Board::TurnCounter() const noexcept
{
    return turn_counter_;
}

size_t Board::PassiveTurnCounter() const noexcept
{
    return passive_turn_counter_;
}

Position Board::LastPawnMove() const noexcept
{
    if(last_pawn_move_.Valid())
        if(current_player_color_ == Color::kWhite)
            return Position(last_pawn_move_.x()-1,last_pawn_move_.y());
        else
            return Position(last_pawn_move_.x()+1,last_pawn_move_.y());
    else
        return Position();
}

void Board::Set(Position position, Cell cell)
{
    assert(position.Valid());
    board_[position.Value()] = cell;
}

void Board::Swap(Position p1, Position p2)
{
    assert(p1.Valid());
    assert(p2.Valid());
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
    assert(position.Valid());
    return board_[position.Value()].type == figure;
}

bool Board::TestColor(Color color, Position position) const noexcept
{
    assert(position.Valid());
    return board_[position.Value()].color == color;
}

bool Board::TestEmp(Position position) const noexcept
{
    assert(position.Valid());
    return board_[position.Value()].type == Figure::kEmpty;
}

Board::Cell Board::GetCell(Position position) const noexcept
{
    assert(position.Valid());
    return board_[position.Value()];
}

Figure Board::GetFigure(Position position) const noexcept
{
    assert(position.Valid());
    return board_[position.Value()].type;
}

Color Board::GetColor(Position position) const noexcept
{
    assert(position.Valid());
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
