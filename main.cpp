#include <iostream>
#include "board.h"
#include "turn.h"
#include "map_preview.h"
#include "turnexecutor.h"
#include "bot.h"

using namespace std;

int main()
{

    Bot b1(white);

    Board b;

    Map_preview m(b,black);
    std::thread th(&m.Exec,&m);
    th.detach();

    for (int i=0;;i++)
    {
        b1.UploadMap(b,Turn());
        Turn t = b1.GetTurn();
        if(!t.Normal())
            break;
        TurnExecutor::ExecuteTurn(b,t);

        if(i%2==0)
            m.UploadMap(b.AfterRotate());
        else
            m.UploadMap(b);
    }

    while(true){}


    return 0;
}
