 #include <iostream>
#include <MyBot.h>
#include <ChessController.h>
#include "UI.h"
#include <UIobs.h>

using namespace std;

Chess::AbstractBot* ConfigurePlayer()
{
    cout << "Choise bot type:" << endl;
    cout << "1)bot" << endl;
    cout << "2)UI" << endl;
    int id = -1;
    cin >> id;
    switch (id) {
    case 1:
        return (new Chess::MyBot);
        break;
    case 2:
        return (new Chess::InputUI);
        break;
    default:
        break;
    }


}

/*
std::vector<Chess::AbstractObserver*> ConfigureObs()
{
    cout << "Choise Your Observer:" << endl;
    cout << "0) exit" << endl;
    cout << "1) History logger" << endl;
}
*/

int main()
{	
    Chess::ChessController c(ConfigurePlayer(),ConfigurePlayer());
    std::vector<Chess::AbstractObserver*> obs;
    obs.push_back(new Chess::ObserverIU);
    c.SetObs(obs);
	
    while(1){c.NextMove();}

    cin.get();
    return 0;
}
