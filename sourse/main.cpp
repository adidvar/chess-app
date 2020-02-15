#include <iostream>
#include <SFML/Graphics.hpp>
#include "ChessController.h"
#include "UI.h"
#include "NetPlayer.h"
#include "MyBot.h"
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

using namespace std;

int main(int argc , char ** argv)
{
    Chess::APlayer * p1, *p2;
    string message = "";

    std::stringstream ss;
    for(int i=0;i<argc;i++)
        ss << argv[i] << " ";
    cout << ss.str() << endl;


    while(ss >> message)
    {
        if(message==string("-p1") || message==string("-p2"))
        {
            auto wt = (message==string("-p1") ? Chess::Color::White : Chess::Color::Black);
            Chess::APlayer *cp;

            string pname;ss >> pname;

            if(pname == string("ai")){
                 cp = new Chess::MyBot(Chess::AI_Mode::Hard,wt);
            }else if(pname == string("ui"))
            {
                cp = new Chess::InputUI(wt,new sf::RenderWindow());
            } else if (pname == string("net")){
                string modestr;ss >> modestr; bool mode;
                if(modestr==string("c"))mode = false;
                if(modestr==string("s"))mode = true;

                if(mode == true){

                    int port;
                    ss >> port;
                    cp = new Chess::NetPlayer(wt,port);
                } else {

                    string ip;
                    int port;
                    ss >> ip >> port;
                    cp = new Chess::NetPlayer(wt,sf::IpAddress(ip),port);
                }
            }
           if(message=="-p1")p1=cp;
           if(message=="-p2")p2=cp;
        }

    }
    {
        Chess::ChessController c(p1,p2);
        while(c.NextMove()){};
    }
    delete p1;
    delete p2;

    return 0;

};
