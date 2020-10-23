#include "map_preview.h"

Map_preview::Map_preview(Board board , Color main_color):
    Player(main_color)
{
    if(main_color != white)
        board.Rotate();
    this->board = board;
}

void Map_preview::UploadMap(Board board)
{
    if(main_color != white)
        board.Rotate();

    std::lock_guard lock(board_mutex);
    this->board = board;
}

void Map_preview::Exec()
{

    static sf::Texture textures[2][7];
    static sf::Sprite sprites[2][7];

    static sf::Font font;

    sf::RenderWindow w(sf::VideoMode(640,640),"Map preview");

    font.loadFromFile("sourse/Font.ttf");

    textures[0][1].loadFromFile("sourse/WPawn.png");
    textures[0][2].loadFromFile("sourse/WKnight.png");
    textures[0][3].loadFromFile("sourse/WBishop.png");
    textures[0][4].loadFromFile("sourse/WRook.png");
    textures[0][5].loadFromFile("sourse/WQueen.png");
    textures[0][6].loadFromFile("sourse/WKing.png");

    textures[1][1].loadFromFile("sourse/BPawn.png");
    textures[1][2].loadFromFile("sourse/BKnight.png");
    textures[1][3].loadFromFile("sourse/BBishop.png");
    textures[1][4].loadFromFile("sourse/BRook.png");
    textures[1][5].loadFromFile("sourse/BQueen.png");
    textures[1][6].loadFromFile("sourse/BKing.png");

    for(int i=0;i<2;i++)
        for(int j=0;j<7;j++)
            sprites[i][j].setTexture(textures[i][j]);

    w.setFramerateLimit(5);

    while (w.isOpen() )
    {
        sf::Event event;
        while (w.pollEvent(event))
            if( (event.type == sf::Event::Closed) )
                w.close();

        w.clear(sf::Color(128,128,128));

        sf::RectangleShape tile(sf::Vector2f(80,80));
        bool is_white = false;
        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                is_white = !is_white;
                tile.setFillColor( is_white ? sf::Color(200,200,200) : sf::Color(50,50,50));
                tile.setPosition(sf::Vector2f(x*80,y*80));
                w.draw(tile);
            }
            is_white = !is_white;
        }

        board_mutex.lock();

        TurnGenerator gen(board);
        sf::CircleShape circle(35);
        circle.setFillColor(sf::Color::Red);

        for(;gen;++gen)
        {
            Turn turn = (*gen);
            if((turn.type == Rooking) || (turn.type == ErrorMove))continue;

            circle.setPosition(sf::Vector2f(Position_y(turn.position_2)*80 + 5,Position_x(turn.position_2)*80+5));

            w.draw(circle);
        }


        sf::Sprite sprite;
        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                sprite = sprites[board.GetColor(Position(x,y))][board.GetFigure(Position(x,y))];
                sprite.setScale(sf::Vector2f(1.3,1.3));
                sprite.setPosition(sf::Vector2f(y*80 + 5,x*80+5));
                if(!board.TestEmp(Position(x,y)))
                    w.draw(sprite);
            }
        }
        board_mutex.unlock();

        w.display();
    }
}
