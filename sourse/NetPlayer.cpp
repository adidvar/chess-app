#include "NetPlayer.h"
#include <iostream>

Chess::Turn Chess::NetPlayer::ToTurn(char *from)
{
    Turn t;
    t.type = (TurnType)from[0];
    if(t.type == Chess::Normal || t.type == Chess::FigureSwap)
    {
        t.start.x = from[1];
        t.start.y = from[2];
        t.end.x = from[3];
        t.end.y = from[4];

        if(t.type == Chess::FigureSwap)
        {
            t.figure = (FigureType)from[5];
        }

    } else
    {
        t.leftright = from[1];
    }
    return t;
}


void Chess::NetPlayer::FromTurn(char *from, Chess::Turn &turn)
{
    from[0] = turn.type;
    if(turn.type == Chess::Normal || turn.type == Chess::FigureSwap)
    {
        from[1] = (char)turn.start.x;
        from[2] = (char)turn.start.y;
        from[3] = (char)turn.end.x;
        from[4] = (char)turn.end.y;

        if(turn.type == Chess::FigureSwap)
        {
            from[5] = turn.figure;
        }
    } else
    {
        from[1] = turn.leftright;
    }
}

Chess::Chessboard Chess::NetPlayer::ToChessboard(char *from)
{
    Chessboard board;

    return board;
}

void Chess::NetPlayer::FromChessboard(char *from, Chess::Chessboard &turn)
{

}

Chess::NetPlayer::NetPlayer(Chess::Color c, sf::IpAddress addr, int port ):
    APlayer(color)
{
    if(this->sock.connect(addr,port) != sf::Socket::Done)exit(1);
    /*
    size_t rec;
    bool mapbyte = MapProp;
    bool hermapbyte;
    sock.send(&mapbyte,1,rec);
    sock.receive(&hermapbyte,1,rec);

    if(mapbyte == false && hermapbyte == true){
        char buf[133];
        sock.receive(buf,133,rec);
        *board = this->ToChessboard(buf);
    } else if (mapbyte == true && hermapbyte == false)
    {
        char buf[133];
        this->FromChessboard(buf,*board);
        sock.send(buf,133,rec);
    } else if (mapbyte == true && hermapbyte == true)
    {
        throw std::string("MapSyncError");
    }
    */
}

Chess::NetPlayer::NetPlayer(Chess::Color c, int port):
    APlayer(color)
{
    sf::TcpListener listener;
    if(listener.listen(port) != sf::TcpSocket::Done)exit(1);
    if(listener.accept(this->sock) != sf::TcpSocket::Done)exit(1);
    /*
     *
     *
    size_t rec;
    bool mapbyte = MapProp;
    bool hermapbyte;
    sock.send(&mapbyte,1,rec);
    sock.receive(&hermapbyte,1,rec);

    if(mapbyte == false && hermapbyte == true){
        char buf[133];
        sock.receive(buf,133,rec);
        *board = this->ToChessboard(buf);
    } else if (mapbyte == true && hermapbyte == false)
    {
        char buf[133];
        this->FromChessboard(buf,*board);
        sock.send(buf,133,rec);
    } else if (mapbyte == true && hermapbyte == true)
    {
        throw std::string("MapSyncError");
    }
*/
    listener.close();

}

bool Chess::NetPlayer::GetTurn(Chess::Turn &turn)
{    

    char buffer[6];
    this->FromTurn(buffer,lastmove);
    this->sock.send(buffer,5);

    size_t rec;
    this->sock.receive(buffer,5,rec);
    turn = this->ToTurn(buffer);

}

void Chess::NetPlayer::MapEvent(Chess::Chessboard map)
{
    lastmove = map.lastturn;
}
