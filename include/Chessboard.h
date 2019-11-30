#pragma once
#include <Chess.h>
#include <array>



namespace Chess{

class MyBot;

class ChessController;

class Chessboard
{
    // чий хід
    bool whiteTurn = true;
    bool LeftRooking = true; // true - можливо зробити рокіровку false - неможливо
    bool RightRooking = true;
    std::array<Figure,64> map;
public:

    friend class Chess::MyBot;


    bool getCurrentTurns(std::vector<Turn> &turns);
    bool makeTurn(Turn turn , bool SafeMode);
    static MatchStatus GetBoardStat(Chessboard &board);

    Chessboard();
    Chessboard(const Chessboard&); // конструктор копіювання
    Chessboard& operator= (const Chessboard &str); // оператор копіювання
    Chessboard operator ~() const;

    Figure& at(unsigned x , unsigned y)
    {
        return (this->map[(x * 8)+y]);
    }
    Figure& at(Position p)
    {
        return (this->map[(p.x * 8)+p.y]);
    }
    bool WhoTurn()
    {
        return whiteTurn;
    }

};

}

