#include "UI.h"

namespace Chess{

    InputUI::InputUI()
    {
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
                turnBuffer.erase(turnBuffer.begin());
                turnBuffer_mtx.unlock();
                break;
            }
            turnBuffer_mtx.unlock();
            _sleep(20);
        }
        return true;
    }

    void InputUI::YouLose()
    {
        stat_mtx.lock();
        stat = Lose;
        stat_mtx.unlock();
    }

    void InputUI::YouWin()
    {
        stat_mtx.lock();
        stat = Win;
        stat_mtx.unlock();
    }

    void InputUI::RenderThread()
    {
        sf::Font font;
        font.loadFromFile("font.ttf");



        std::vector<Position> p;
        sf::Texture textures[2][7];

        //White
        //textures[0][0].loadFromFile("Texture/WEmpty.png");
        textures[0][1].loadFromFile("Texture/WPawn.png");
        textures[0][2].loadFromFile("Texture/WKnight.png");
        textures[0][3].loadFromFile("Texture/WBishop.png");
        textures[0][4].loadFromFile("Texture/WRook.png");
        textures[0][5].loadFromFile("Texture/WQueen.png");
        textures[0][6].loadFromFile("Texture/WKing.png");
        //Black
        //textures[1][0].loadFromFile("Texture/BEmpty.png");
        textures[1][1].loadFromFile("Texture/BPawn.png");
        textures[1][2].loadFromFile("Texture/BKnight.png");
        textures[1][3].loadFromFile("Texture/BBishop.png");
        textures[1][4].loadFromFile("Texture/BRook.png");
        textures[1][5].loadFromFile("Texture/BQueen.png");
        textures[1][6].loadFromFile("Texture/BKing.png");



        sf::RenderWindow window(sf::VideoMode(640,640),"InputUI");
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

           if(GetStat() == Now)
           {

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
           figure.setScale(sf::Vector2f(1.5f,1.5f));
           for (int x = 0; x < 8; x++)
           {
               for (int y = 0; y < 8; y++)
               {
                   unsigned color_id = (unsigned)map.at(x,y).color  , figure_id = (unsigned)map.at(x,y).type;
                   figure.setTexture(textures[color_id][figure_id]);
                   figure.setPosition(sf::Vector2f(y*80,x*80));
                   if( figure_id != Chess::Emply)
                       window.draw(figure);
               }
          }
          map_mtx.unlock();
          }
          else if(GetStat() == Win)
          {
               sf::Text text("You Win!",font,30);
               text.setPosition(260,300);
               window.draw(text);

          }
          else if(GetStat() == Lose)
          {
               sf::Text text("You Lose!",font,30);
               text.setPosition(260,300);
               window.draw(text);
          }

           window.display();
           _sleep(20);

        }

    }

};
