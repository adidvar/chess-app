#include "MainMenu.h"

void MainMenu::My()
{
    srand(std::clock());
    w.create(sf::VideoMode(640,640),"Main");
    w.setFramerateLimit(60);

    sf::VertexArray array(sf::Points,640*640);
    for(int i=0;i<640;i++)
    {
        for(int j=0;j<640;j++)
        {
            array[i*640+j].color = sf::Color::Black;
            array[i*640+j].position = sf::Vector2f(i,j);
        }
    }
    sf::Font f;f.loadFromFile("sourse/Font.ttf");

    sf::Text single;single.setFont(f);single.setCharacterSize(50);single.setPosition(Vector2f(150,150));single.setColor(sf::Color::Black);single.setString("Single");single.setOutlineColor(Color::White);single.setOutlineThickness(1.5);
    sf::Text multi;multi.setFont(f);multi.setCharacterSize(50);multi.setPosition(Vector2f(150,210));multi.setColor(sf::Color::Black);multi.setString("Multi");multi.setOutlineColor(Color::White);multi.setOutlineThickness(1.5);
    sf::Text custom;custom.setFont(f);custom.setCharacterSize(50);custom.setPosition(Vector2f(150,270));custom.setColor(sf::Color::Black);custom.setString("Custom");custom.setOutlineColor(Color::White);custom.setOutlineThickness(1.5);
    int cx , cy;
    int squadness = 400;

    while(w.isOpen())
    {

        cx = rand() % 640;cy = rand() % 640;

        Event e;
        while(w.pollEvent(e))
        {
            if(e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left && e.mouseButton.x > 150 && e.mouseButton.y > 150 && e.mouseButton.y < 330)
            {
                int y = e.mouseButton.y;

                if(y < 210)
                {
                    Chess::Color color = (Chess::Color)(rand() % 2);

                    std::array<Chess::APlayer*,2> pl;
                    pl[0] = new Chess::InputUI(color,&w);
                    pl[1] = new Chess::MyBot(color == Chess::White ? Chess::Black  : Chess::White );

                    Chess::ChessController c(!color ? pl[0] : pl[1] , color ? pl[0] : pl[1]);

                    while(c.NextMove()){;}

                    delete pl[0];
                    delete pl[1];

                    w.create(sf::VideoMode(640,640),"Main");
                    w.setFramerateLimit(60);
                }
                else if (y < 270)
                {
                }
                else
                {
                }

            }
        }



        w.clear(sf::Color::Black);
        //фон
        if(squadness > -1000){
        for(int i=0;i<640;i++)
        {
            for(int j=0;j<640;j++)
            {
                array[i*640+j].color.r += sqrt((pow(j-cx,2)+pow(i-cy,2)))/squadness;
                array[i*640+j].color.g += sqrt((pow(j-cx,2)+pow(i-cy,2)))/squadness;
                array[i*640+j].color.b += sqrt((pow(j-cx,2)+pow(i-cy,2)))/squadness;
            }
        }
        squadness--;}
        w.draw(array);
        w.draw(single);
        w.draw(multi);
        w.draw(custom);
        w.display();
    }
}

MainMenu::MainMenu()
{
    My();
}

MainMenu::~MainMenu()
{
}
