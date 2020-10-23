#ifndef BOARD_H
#define BOARD_H

#include <bitset>
#include <array>
#include <sstream>

using Pos = uint8_t;

const Pos NANPOS = 64;

constexpr Pos Position(Pos p1 , Pos p2)
{
    if( p1 > 7 || p2 > 7)
        return NANPOS;

    return p1*8 + p2;
}

constexpr bool PositionNormal(Pos test)
{
    return test != NANPOS;
}

constexpr Pos Position_x(Pos p1)
{
    return p1/8;
}
constexpr Pos Position_y(Pos p1)
{
    return p1%8;
}

using Figures = uint8_t;
enum Figures_enum
{
    empty = 0,
    pawn = 1,
    knight = 2,
    bishop = 3,
    rook = 4,
    queen = 5,
    king = 6
};

using Color = uint8_t;
enum Color_enum
{
    white = 0,
    black = 1
};

class Board
{

    struct figure
    {
        figure():
            type(empty),
            color(white)
        {}
        uint8_t type:3;
        uint8_t color:1;
    };

    std::array<figure,64> map;
    bool white_black = 0;
public:
    void Rotate();
    bool GetRotate();
    Board AfterRotate();

public:
    Board(); // default cheese map
    Board(std::string from_string); // default cheese map
    Board(char *data); // import from zipped formatss

    Board& operator =(const Board& b) = default;
    Board(const Board&) = default;
    Board( Board&&) = default;

    bool Test(Figures figure , Pos position) const;
    bool TestColor(Color color ,Pos position) const;
    bool TestEmp(Pos position) const;

    void Set(Pos position , Figures figure , Color c );
    void Swap(Pos p1 , Pos p2);

    Figures GetFigure(Pos position) const;
    Color GetColor(Pos position) const;

    std::string toStr();

    friend class TurnExecutor;
};

#endif // BOARD_H
