#ifndef NETPLAYER_H
#define NETPLAYER_H
#include "APlayer.h"
#include <SFML/Network.hpp>
#define TURNSIZE 6
//protocall

namespace Chess
{

class NetPlayer : public APlayer
{
    Turn lastmove;
    sf::TcpSocket sock;

public:
    NetPlayer(Color c, sf::IpAddress , int port);
    NetPlayer(Color c, int port);
    bool GetTurn(Turn &turn);
    void MapEvent(Chess::Chessboard map);
    void FinishEvent(Chess::Color){};


    static Turn ToTurn(char * from);
    static void FromTurn(char * from , Turn &turn);
    static Chessboard ToChessboard(char * from);
    static void FromChessboard(char * from , Chessboard &turn);
};

#endif // NETPLAYER_H

};
