#include <iostream>
#include "board.h"
#include "turn.h"
#include "map_preview.h"
#include "turnexecutor.h"
#include "bot.h"

using namespace std;

int main()
{
    Bot b2(white);

    Board b;

    Map_preview b1(b,black);
    std::thread th(&Map_preview::Exec,&b1);
    th.detach();

    for (int i=0;;i++)
    {
        {
            b1.UploadMap(b,Turn());
            Turn t = b1.GetTurn();
            if(!t.Normal())
                break;
            TurnExecutor::ExecuteTurn(b,t);
        }
        {
            b2.UploadMap(b,Turn());
            Turn t = b2.GetTurn();
            if(!t.Normal())
                break;
            TurnExecutor::ExecuteTurn(b,t);
        }
    }

    while(true){}


    return 0;
}
