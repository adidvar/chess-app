#include <iostream>
#include <string>
#include <chrono>
#include <board.h>
#include <bitboard.h>
#include <magicboardtools.h>

template<class Board>
static size_t MovesCounter(Board board,size_t depth){
    if(!depth)
        return 1;
    size_t counter = 0;

    for(auto board: board.GenerateSubBoards())
        counter += MovesCounter(board,depth-1);

    return counter;
}

void SeeBoard(Board board , size_t depth)
{
    using namespace std;
    for(auto turn : board.GenerateTurns()){
        Board copy(board);
        copy.ExecuteTurn(turn);
        cout << turn.ToChessFormat() << ": " << MovesCounter(copy,depth-1) << endl;
    }
    cout << MovesCounter(board,depth) << endl;
}

void PrintBoard(uint64_t board);

void BenchMaarkBoards(){

    using namespace std::chrono;
    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << MovesCounter<Board>(Board(),4) << std::endl;
    auto end = std::chrono::high_resolution_clock::now() - begin;
    std::cout << "Board:" << std::chrono::duration_cast<std::chrono::milliseconds>(end).count() << std::endl;

    begin = std::chrono::high_resolution_clock::now();
    std::cout << MovesCounter<BitBoard>(BitBoard(),4) << std::endl;
    end = std::chrono::high_resolution_clock::now() - begin;
    std::cout << "BitBoard:" << std::chrono::duration_cast<std::chrono::milliseconds>(end).count() << std::endl;

}


int main()
{
    BenchMaarkBoards();
    using namespace std;
    /*
    std::string fen;
    std::getline(std::cin,fen);
    Board board(fen);
    std::cout << board.Fen();
    BitBoard board("rnb1kbnr/pp1ppppp/8/8/8/p6q/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    board.PrintBoard();
    for(auto board : board.GenerateSubBoards()){
        board.PrintBoard();
    }
    */
/*
    uint64_t mask = 1 + (1<<9) + (1<<18) + (1<<27) + (1LL<<36);
    PrintBoard(mask);
    auto magic = FindMagic(mask);
    std::cout << magic.first << ' ' << magic.second << std::endl;
    while(true){
        size_t num;
        std::cin >> num;
        PrintBoard(((num*magic.first) >> magic.second)&mask);
    }

    */
    /*
    size_t depth;
    cin >> depth;
    SeeBoard(board,depth);
    while(true){
        int move;
        cin >> move;
        auto turn = board.GenerateTurns()[move];
        board.ExecuteTurn(turn);
        depth--;
    SeeBoard(board,depth);
    }
    */
    return 0;
}
