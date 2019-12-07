 #include <iostream>
#include <SFML/Graphics.hpp>
#include "ChessController.h"
#include "MyBot.h"
#include "UI.h"

using namespace std;

int main()
{	
    sf::RenderWindow w(sf::VideoMode(640,640),"title");
    w.setActive(false);
    Chess::MyBot bot(Chess::AI_Mode::Hard,Chess::Color::White);
    Chess::InputUI ui(&w,Chess::Color::Black);
    Chess::ChessController c(&bot,&ui);
    while(c.NextMove()){}
    w.setActive(true);
    cout << "returns to main" << endl;
    cin.get();
    return 0;
};
