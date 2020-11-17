#include "map_preview.h"

Map_preview::Map_preview(Board board , Color main_color):
    Player(main_color)
{
    if(main_color != white)
        board.Rotate();
    this->board = board;
}

void Map_preview::UploadMap(Board board, Turn lastturn)
{
    if(main_color != white)
        board.Rotate();

    std::lock_guard lock(board_mutex);
    this->board = board.AfterRotate();
}

Turn Map_preview::GetTurn()
{
    handle_flag = true;

    std::unique_lock lock(this->turn_mutex);

    do {
        handle.wait(lock);
    } while (!turn.Normal());

    auto result = turn;
    turn = Turn();

    handle_flag = false;

    return result;
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
        this->CaptureEvent(w);

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

//            w.draw(circle);
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

void Map_preview::CaptureEvent(sf::RenderWindow &w)
{
    static std::vector<Pos> clicks;

    sf::Event e;
    while(w.pollEvent(e))
    {
        if(e.type == sf::Event::Closed)
            w.close();
        if(e.type == sf::Event::MouseButtonPressed && handle_flag && clicks.size()<2)
        {
            auto info = e.mouseButton;
            auto pos = Position(info.y / 80 , info.x / 80);

            std::lock_guard guard(board_mutex);
            if(( (!board.TestEmp(pos) && board.TestColor(white,pos) && clicks.size() == 0))
                    || ((board.TestColor(black,pos) || board.TestEmp(pos)) && clicks.size() == 1))
                clicks.push_back(pos);
            else
                clicks.clear();
        }
    }
    //кінцевий тест на правильність ходу
    if(clicks.size() == 2){

        std::lock_guard guard(board_mutex);
        turn_mutex.lock();

        Turn tests_turn;

        if(Position_x(clicks[0]) ==1 && board.Test(pawn,clicks[0]))
            tests_turn = Turn(clicks[0],clicks[1],PickRender(w));
        else if (false) // тест на рокіровку
        {

        } else
            tests_turn = Turn(clicks[0],clicks[1]);

        if(TurnExecutor::TurnTest(board,tests_turn))
        {
            this->turn = tests_turn;
            turn_mutex.unlock();
            handle.notify_all();
            clicks.clear();
            TurnExecutor::ExecuteTurn(this->board,tests_turn);
            this->board.Rotate();
            return;
        }
        turn_mutex.unlock();
        clicks.clear();
    }
}

Figures Map_preview::PickRender(sf::RenderWindow &w)
{
    return queen;
}
