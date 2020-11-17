#include "board.h"

#define N(p) (white_black ? (63-p) : (p))
#define P(p) (white_black ? (!p) : (p))

void Board::Rotate(){white_black = !white_black;}

bool Board::GetRotate(){return white_black;}

Board Board::AfterRotate()
{
    Board after(*this);
    after.Rotate();
    return after;
}


const Figures default_map_figures[64] = {rook , knight , bishop , queen , king , bishop , knight , rook ,
                                         pawn , pawn , pawn ,pawn, pawn , pawn , pawn ,pawn,
                                         empty , empty , empty , empty ,empty , empty , empty , empty,
                                         empty , empty , empty , empty ,empty , empty , empty , empty,
                                         empty , empty , empty , empty ,empty , empty , empty , empty,
                                         empty , empty , empty , empty ,empty , empty , empty , empty,
                                         pawn , pawn , pawn ,pawn, pawn , pawn , pawn ,pawn,
                                         rook , knight , bishop , queen , king , bishop , knight , rook , };

const Figures default_map_colores[64] = {black , black , black , black , black , black , black , black ,
                                          black , black , black , black , black , black , black , black ,
                                          black , black , black , black , black , black , black , black ,
                                          black , black , black , black , black , black , black , black ,
                                          white , white ,white ,white ,white ,white ,white ,white ,
                                          white , white ,white ,white ,white ,white ,white ,white ,
                                          white , white ,white ,white ,white ,white ,white ,white ,
                                          white , white ,white ,white ,white ,white ,white ,white , };


Board::Board()
{
    for(uint8_t i = 0 ; i < 64 ; ++i)
        this->Set(i,default_map_figures[i] , default_map_colores[i] );
}

bool Board::Test(Figures figure, Pos position) const
{
    return map[N(position)].type == figure;
}

bool Board::TestColor(Color color, Pos position) const
{
    return P(map[N(position)].color) == color;
}

bool Board::TestEmp(Pos position) const
{
    return map[N(position)].type == empty;
}

void Board::Set(Pos position, Figures figure, Color c)
{
    map[N(position)].color = P(c);
    map[N(position)].type = figure;
}

void Board::Swap(Pos p1, Pos p2)
{
    std::swap(map[N(p1)],map[N(p2)]);
}

Figures Board::GetFigure(Pos position) const
{
    return map[N(position)].type;
}

Color Board::GetColor(Pos position) const
{
    return P(map[N(position)].color);
}

std::array<char , 14> str_coding = {' ','p','n','b','r','q','k' , ' ','P','N','B','R','Q','K'};

std::string Board::toStr()
{
    std::stringstream ss;
    for(size_t x = 0 ; x < 8 ; x++)
    {
        for(size_t y = 0 ; y < 8 ; y++)
        {
            ss << str_coding[GetFigure(Position(x,y)) + 7*(GetColor(Position(x,y)))];
        }
        ss << std::endl;
    }
    return ss.str();
}
