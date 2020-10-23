#include <iostream>
#include "board.h"

using namespace std;

unsigned ok = 0;
unsigned error = 0;

void Ok()
{
    cout << "<=OK" << endl;
    ok++;
}

void Error()
{
    cout << "<=Error" << endl;
    error++;
}

int main()
{
    cout << "board <= test start" << endl;

    Board board;

    for(int i=0;i<64;i++)
    {
        Figures fig = 0;
        Color color = 0;

        for(Figures fig = 0; fig < 7 ; fig++){
            for(Color color = 0; color < 2; color++){

                board.Set(i,fig,color);

                if(board.Test(fig,i))
                    Ok();
                else
                    Error();

                if(board.TestColor(color,i))
                    Ok();
                else
                    Error();

                if(board.GetFigure(i) == fig)
                    Ok();
                else
                    Error();

                if(board.GetColor(i) == color)
                    Ok();
                else
                    Error();
            }
        }
    }

    board.Set(0,pawn,white);
    board.Set(1,knight,black);
    board.Swap(0,1);

    if(board.Test(pawn,1))
        Ok();
    else
        Error();

    if(board.Test(knight,0))
        Ok();
    else
        Error();

    if(board.TestColor(black,0))
        Ok();
    else
        Error();

    if(board.TestColor(white,1))
        Ok();
    else
        Error();


    cout << "Error:" << error << endl;
    cout << "Ok:" << ok << endl;

    return 0;
}
