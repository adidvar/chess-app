#include "UI.h"
#include <iostream>

namespace Chess{

    InputUI::InputUI()
    {
        //White
        textures[0][0].loadFromFile("Texture/WEmpty.png");
        textures[0][1].loadFromFile("Texture/WPawn.png");
        textures[0][2].loadFromFile("Texture/WKnight.png");
        textures[0][3].loadFromFile("Texture/WBishop.png");
        textures[0][4].loadFromFile("Texture/WRook.png");
        textures[0][5].loadFromFile("Texture/WQueen.png");
        textures[0][6].loadFromFile("Texture/WKing.png");
        //Black
        textures[1][0].loadFromFile("Texture/BEmpty.png");
        textures[1][1].loadFromFile("Texture/BPawn.png");
        textures[1][2].loadFromFile("Texture/BKnight.png");
        textures[1][3].loadFromFile("Texture/BBishop.png");
        textures[1][4].loadFromFile("Texture/BBishop.png");
        textures[1][5].loadFromFile("Texture/BRook.png");
        textures[1][6].loadFromFile("Texture/BKing.png");

        std::thread th(&InputUI::RenderThread,this);
        th.detach();
    }

    bool InputUI::GetTurn(Chess::Turn &t,Chess::Chessboard &board)
    {
        map_mtx.lock();
         map = board;
        map_mtx.unlock();
        while(1){
            turnBuffer_mtx.lock();
            if(turnBuffer.size() > 0){
                t = turnBuffer.front();
                std::cout << t.start.x << t.start.y << t.end.x << t.end.y << std::endl;
                turnBuffer.erase(turnBuffer.begin());
                turnBuffer_mtx.unlock();
                break;
            }
            turnBuffer_mtx.unlock();
            _sleep(20);
        }
        return true;
    }

    void InputUI::RenderThread()
    {
        std::vector<Position> p;

        sf::RenderWindow window(sf::VideoMode(640,640),"InputUI");
        window.setFramerateLimit(60);
        while (window.isOpen())
        {
           sf::Event event;
           while (window.pollEvent(event))
           {
               if (event.type == sf::Event::Closed)
                   window.close();
               if(event.type == sf::Event::MouseButtonPressed)
                {
                    p.push_back(Position(event.mouseButton.y / 80,event.mouseButton.x / 80));

                    if(p.size() >= 2)
                    {
                        turnBuffer_mtx.lock();
                        turnBuffer.push_back(Turn(p[0],p[1]));
                        p.clear();
                        turnBuffer_mtx.unlock();
                    }
                }
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
           sf::Sprite figure;
           figure.setScale(sf::Vector2f(1.5f,1.5f));
           map_mtx.lock();
           for (int x = 0; x < 8; x++)
           {
               for (int y = 0; y < 8; y++)
               {
                   figure.setTexture(textures[(int)map.at(x,y).color][(int)map.at(x,y).type]);
                   figure.setPosition(sf::Vector2f(y*80,x*80));
                   window.draw(figure);
               }
           }
           map_mtx.unlock();

           window.display();
           _sleep(20);
        }

    }

};
