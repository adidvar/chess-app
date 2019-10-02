#include <iostream>
#include <MyBot.h>

using namespace std;

int main()
{	
	Chess::MyBot b1;
    InputUI b2;
	Chess::ChessController c(b1,b2);
	
    while(1){
		c.NextMove();
	}
    return 0;
}
