#include <iostream>
#include <SFML/Graphics.hpp>
#include "ChessController.h"
#include "UI.h"
#include "NetPlayer.h"
#include "MyBot.h"
#include <string>
#include <vector>
#include <cstring>

using namespace std;

int main()
{	
    Chess::APlayer * p1, *p2;
    string message = "";

    std::cout << "Welcome in Chess-app\nEnter settings in format: p1 {PlayerType} {PlayerArguments} p2 {PlayerType} {PlayerArguments} start\nSuported PlayerType: ai(bot) ui(GUI) net(remote interface)\nFor example (p1 ui p2 ai start) for play with bot" << std::endl;

    while(message!=string("start"))
    {
        cin >> message;
        if(message==string("p1") || message==string("p2"))
        {
            auto wt = (message==string("p1") ? Chess::Color::White : Chess::Color::Black);
            Chess::APlayer *cp;

            string pname;cin >> pname;

            if(pname == string("ai")){
                 cp = new Chess::MyBot(Chess::AI_Mode::Hard,wt);
            }else if(pname == string("ui"))
            {
                cp = new Chess::InputUI(wt);
            } else if (pname == string("net")){
                string modestr;cin >> modestr; bool mode;
                if(modestr==string("c"))mode = false;
                if(modestr==string("s"))mode = true;

                if(mode == true){

                    int port;
                    cin >> port;
                    cp = new Chess::NetPlayer(wt,port);
                } else {

                    string ip;
                    int port;
                    cin >> ip >> port;
                    cp = new Chess::NetPlayer(wt,sf::IpAddress(ip),port);
                }
            }
           if(message=="p1")p1=cp;
           if(message=="p2")p2=cp;
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
