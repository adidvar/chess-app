#include <UIobs.h>
#include <iostream>

namespace Chess{

    ObserverIU::ObserverIU()
    {
        std::thread th(&ObserverIU::RenderThread,this);
        th.detach();
    }

    void ObserverIU::MapEvent(Chess::Chessboard board)
    {
        map_mtx.lock();
         map = board;
        map_mtx.unlock();
    }

    void ObserverIU::RenderThread()
    {

        std::vector<Position> p;
        sf::Texture textures[2][7];
        sf::Sprite sprites[2][7];
        const int twidth = 50;
        const int theight = 50;

        //White
        //textures[0][0].loadFromFile("Texture/WEmpty.png");
        textures[0][1].loadFromFile("sourse/WPawn.png");
        textures[0][2].loadFromFile("sourse/WKnight.png");
        textures[0][3].loadFromFile("sourse/WBishop.png");
        textures[0][4].loadFromFile("sourse/WRook.png");
        textures[0][5].loadFromFile("sourse/WQueen.png");
        textures[0][6].loadFromFile("sourse/WKing.png");
        //Black
        //textures[1][0].loadFromFile("sourse/BEmpty.png");
        textures[1][1].loadFromFile("sourse/BPawn.png");
        textures[1][2].loadFromFile("sourse/BKnight.png");
        textures[1][3].loadFromFile("sourse/BBishop.png");
        textures[1][4].loadFromFile("sourse/BRook.png");
        textures[1][5].loadFromFile("sourse/BQueen.png");
        textures[1][6].loadFromFile("sourse/BKing.png");

        for(int i=0;i<2;i++)
        {
            for(int j=0;j<7;j++)
            {
                sprites[i][j].setTexture(textures[i][j]);
            }
        }

        sf::RenderWindow window(sf::VideoMode(640,640),"Observer");
        while (window.isOpen())
        {
           sf::Event event;
           while (window.pollEvent(event))
           {
               if (event.type == sf::Event::Closed)
                   window.close();
           }
           window.clear();

           sf::RectangleShape MapRectangle(sf::Vector2f(80,80));
           bool IsWhite = false;
           for (int x = 0; x < 8; x++)
           {
               for (int y = 0; y < 8; y++)
               {
                   IsWhite = !IsWhite;
                   MapRectangle.setFillColor( IsWhite ? sf::Color(200,200,200) : sf::Color(50,50,50));
                   MapRectangle.setPosition(sf::Vector2f(x*80,y*80));
                   window.draw(MapRectangle);
               }
               IsWhite = !IsWhite;
           }

           map_mtx.lock();
           sf::Sprite figure;
           figure.setScale(sf::Vector2f((float)70/twidth,(float)70/theight));
           for (int x = 0; x < 8; x++)
           {
               for (int y = 0; y < 8; y++)
               {
                   auto fgr = map.at(x,y);
                   int color_id = fgr.color ; int figure_id = fgr.type;
                   sprites[color_id][figure_id].setScale(sf::Vector2f((float)70/twidth,(float)70/theight));
                   sprites[color_id][figure_id].setPosition(sf::Vector2f(y*80 + 5,x*80+5));
                   if( figure_id != Chess::Emply)
                       window.draw(sprites[color_id][figure_id]);
               }
          }
          map_mtx.unlock();

           window.display();
           _sleep(20);

        }

    }

};
