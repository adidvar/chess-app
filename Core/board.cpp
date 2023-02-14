#include "board.h"
#include "turngenerator.h"
#include <stdexcept>
#include <sstream>

const char* Board::fen_start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq — 0 1";

Board::Board(std::string_view fen)
{
    for(size_t i = 0 ; i < 64 ; i++)
        Set(i,empty,white);

    size_t i = 0;
    size_t position = 0;
    while(i < fen.size() && position != 64 && fen[i]!=' ')
    {
        switch (fen.at(i)) {
        case 'p':
            Set(position,pawn,black);
            position++;
            break;
        case 'n':
            Set(position,knight,black);
            position++;
            break;
        case 'b':
            Set(position,bishop,black);
            position++;
            break;
        case 'r':
            Set(position,rook,black);
            position++;
            break;
        case 'q':
            Set(position,queen,black);
            position++;
            break;
        case 'k':
            Set(position,king,black);
            position++;
            break;
        case 'P':
            Set(position,pawn,white);
            position++;
            break;
        case 'N':
            Set(position,knight,white);
            position++;
            break;
        case 'B':
            Set(position,bishop,white);
            position++;
            break;
        case 'R':
            Set(position,rook,white);
            position++;
            break;
        case 'Q':
            Set(position,queen,white);
            position++;
            break;
        case 'K':
            Set(position,king,white);
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
        this->current_color = white;
    else if(current_move == "b")
        this->current_color = black;
    else
        throw std::runtime_error("fen invalid format [current_move]");
    this->static_move_counter = static_move;
    this->turn_counter = move_counter;

    this->r_flags = {false,false,false,false};

    for(char x : rooking)
    {
        switch (x) {
        case 'K':
            r_flags.white_oo = true;
            break;
        case 'Q':
            r_flags.white_ooo = true;
            break;
        case 'k':
            r_flags.black_oo = true;
            break;
        case 'q':
            r_flags.black_ooo = true;
            break;
        case '-':
            break;
        default:
            throw std::runtime_error("fen invalid format [rooking]");
        }
    }
}

Board::Board(const std::initializer_list<std::pair<Figures, Color> > list)
{
    if(list.size() != 64)
        throw std::runtime_error("Board inizializer list error");
    size_t index = 0;
    for(  auto it = list.begin() ; it != list.end() ; ++it )
    {
        this->Set( index , (*it).first , (*it).second );
        ++index;
    }
}

void Board::Set(Pos position, Figures figure, Color c)
{
    map[position].color = c;
    map[position].type = figure;
}

void Board::Swap(Pos p1, Pos p2)
{
    std::swap(map[p1],map[p2]);
}

bool Board::Test(Figures figure, Pos position) const
{
    return map[position].type == figure;
}

bool Board::TestColor(Color color, Pos position) const
{
    return map[position].color == color;
}

bool Board::TestEmp(Pos position) const
{
    return map[position].type == empty;
}

Figures Board::GetFigure(Pos position) const
{
    return map[position].type;
}

Color Board::GetColor(Pos position) const
{
    return map[position].color;
}

Board::operator bool()
{
    return TurnGenerate(*this,current_color).size() != 0;
}
