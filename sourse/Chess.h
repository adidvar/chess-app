#pragma once
#include <vector>
#include <assert.h>

namespace Chess{

enum FigureType
{
    Emply = 0,
    Pawn = 1,
    Knight = 2,
    Bishop = 3,
    Rook = 4,
    Queen = 5,
    King = 6
};

enum MatchStatus
{
    Now,
    Win,
    Lose
};


enum Color
{
    White = 0,
    Black = 1
};

struct Figure
{
    Figure(FigureType t , Color c):
        type(t),
        color(c)
    {}
    Figure(){}
    FigureType type;
    Color color;
};

struct Position
{
    Position():
        x(0),
        y(0){}
    Position(int xx , int yy):
        x(xx),
        y(yy){}
    int x;
    int y;
};

enum TurnType
{
    Normal,
    Rooking,
    FigureSwap
};

struct Turn
{
    Turn(Position s , Position e): // normal
        type(Normal),start(s),end(e)
    {}
    Turn( bool lr ):
        type(Rooking),leftright(lr) // Rooking
    {}
    Turn( Position s , Position e , FigureType fgr ): // Swap
        type(FigureSwap),start(s),end(e),figure(fgr)
    {}
    Turn(): // Zero
        type(Normal),start(),end()
    {}

    TurnType type;

    bool leftright;

    Position start;
    Position end;

    FigureType figure;

};

bool operator==(const Position &t1 , const Position &t2);
bool operator==(const Turn &t1 , const Turn &t2);

}

