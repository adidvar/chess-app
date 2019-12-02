 #include <iostream>
#include "ChessController.h"
#include "MyBot.h"
#include "UI.h"

using namespace std;

int main()
{	
    Chess::MyBot bot(Chess::AI_Mode::Hard,Chess::Color::White);
    Chess::InputUI ui(Chess::Color::Black);
    Chess::ChessController c(&bot,&ui);
	
    while(1){c.NextMove();}

    cin.get();
    return 0;
};
