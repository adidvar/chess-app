#include <iostream>
#include "turngenerator.h"
#include "turn.h"
#include "ctime"

using namespace std;

unsigned long long turns = 1;

unsigned long long tmm = 1;

unsigned long long t1;
unsigned long long t2;

int main()
{
    Board b;

    for(int i=0;1;i++){

//        cout << b.toStr() << endl;

//        b.Set(Position(rand()%8 , rand()%8),rand()%7 , rand()%2);

        t1 = clock();


        TurnGenerator t(b);

        for(;t;++t)
        {
           cout << (*t).toStr() << endl;
           turns++;
           (*t);
        }

        t2 = clock();

        tmm += t2-t1;

        if(i%10000==0){
            cout << turns/tmm << endl;
            tmm = 1;
            turns = 0;
        }
        break;

    }




    return 0;
}
