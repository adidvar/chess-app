#include "UI.h"

namespace Chess{

    InputUI::InputUI(Color c):
        APlayer(c)
    {
        std::thread th(&InputUI::RenderThread,this);
        th.detach();
    }

    void InputUI::MapEvent(Chessboard board)
    {
        map_mtx.lock();
         map = board;
        map_mtx.unlock();
    }

    bool InputUI::GetTurn(Chess::Turn &t)
    {
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
        font.loadFromFile("sourse/Font.ttf");

        bool FigureChoiseMenu = false;

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



        sf::RenderWindow window(sf::VideoMode(640,640),"InputUI");
        while (window.isOpen())
        {
           sf::Event event;
           while (window.pollEvent(event))
           {
               if (event.type == sf::Event::Closed)
                   window.close();
               if(event.type == sf::Event::MouseButtonPressed && FigureChoiseMenu == false)
                {
                    p.push_back(Position(event.mouseButton.y / 80,event.mouseButton.x / 80));

                    if(p.size() >= 2)
                    {
                        map_mtx.lock();
                        if(p[0] == Position(7,4) && (p[1] == Position(7,2) || p[1] == Position(7,6))) // рокіровка
                        {
                            turnBuffer_mtx.lock();
                            if(p[1] == Position(7,2))
                            {
                                turnBuffer.push_back(Turn(true));
                            } else if (p[1] == Position(7,6))
                            {
                                turnBuffer.push_back(Turn(false));
                            }
                            p.clear();
                            turnBuffer_mtx.unlock();

                        } else if (p[0].x == 1 && map.at(p[0]).type == Pawn )
                        {
                            FigureChoiseMenu = true;
                        } else {
                            turnBuffer_mtx.lock();
                            turnBuffer.push_back(Turn(p[0],p[1]));
                            p.clear();
                            turnBuffer_mtx.unlock();
                        }
                        map_mtx.unlock();
                    }
                }
               if(event.type == sf::Event::MouseButtonPressed && FigureChoiseMenu == true)
                {
                    int x = event.mouseButton.x , y = event.mouseButton.y;

                    if( y > 320 && y < 420){
                        if( x > 105 && x < 205)
                        {
                            turnBuffer_mtx.lock();
                            turnBuffer.push_back(Turn(p[0],p[1],Pawn));
                            p.clear();
                            turnBuffer_mtx.unlock();
                            FigureChoiseMenu = false;
                        } else if ( x > 215 && x < 315 )
                        {
                            turnBuffer_mtx.lock();
                            turnBuffer.push_back(Turn(p[0],p[1],Knight));
                            p.clear();
                            turnBuffer_mtx.unlock();
                            FigureChoiseMenu = false;
                        } else if ( x > 325 && x < 425 )
                        {
                            turnBuffer_mtx.lock();
                            turnBuffer.push_back(Turn(p[0],p[1],Bishop));
                            p.clear();
                            turnBuffer_mtx.unlock();
                            FigureChoiseMenu = false;

                        }else if( x > 435 && x < 535  )
                        {
                            turnBuffer_mtx.lock();
                            turnBuffer.push_back(Turn(p[0],p[1],Queen));
                            p.clear();
                            turnBuffer_mtx.unlock();
                            FigureChoiseMenu = false;
                        }
                    }
                }
           }
           window.clear();

           if(GetStat() == Now && FigureChoiseMenu == false)
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
          else if(GetStat() == Now && FigureChoiseMenu == true)
          {
            sf::Text text("Choise figure",font,30);
            text.setFont(font);
            text.setPosition(240,260);
            window.draw(text);

            sf::Sprite figure;
            figure.setScale((float)100/twidth,(float)100/theight);

            figure.setTexture(textures[0][2]);
            figure.setPosition(105,320);
            window.draw(figure);

            figure.setTexture(textures[0][3]);
            figure.setPosition(215,320);
            window.draw(figure);

            figure.setTexture(textures[0][4]);
            figure.setPosition(325,320);
            window.draw(figure);

            figure.setTexture(textures[0][5]);
            figure.setPosition(435,320);
            window.draw(figure);
          }

           window.display();
           _sleep(20);

        }

    }

};
