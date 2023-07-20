#include <iostream>
#include <vector>
#include <sstream>
#include <asyncio.hpp>

#include <turn.hpp>

//to controller
void Id(){
    std::cout << "id name Chess5" << std::endl;
    std::cout << "id author adidvar@gmail.com" << std::endl;
};

void Uciok(){
    std::cout << "uciok" << std::endl;
};

void Readyok(){
    std::cout << "readyok" << std::endl;
};

void BestMove(Turn turn){
    std::cout << "bestmove " << turn.ToChessFormat() << std::endl;
};

/*
void copyprotection(bool ok){
    std::cout << "copyprotection " <<
             (ok ? (const char*)"ok" : (const char*)"error")
          << std::endl;
};
void registration(bool ok){
    std::cout << "registration " <<
             (ok ? (const char*)"ok" : (const char*)"error")
          << std::endl;
};
void info(){

};
void option(){
};
*/

struct UCIState{

    //from controller
    void uci();
    void debug();
    void isready();
    void setoption();
    void registered();
    void ucinewgame();
    void position();
    void go();
    void stop();
    void ponderhit();
    void quit(){
        exit(0);
    };

};

int main(){
    Asyncio io;
    UCIState state;


}
