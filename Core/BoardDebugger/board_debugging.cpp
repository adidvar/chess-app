#include <iostream>
#include <string>
#include <board.h>

static size_t MovesCounter(Board board,size_t depth){
    if(!depth)
        return 1;
    auto moves = board.GenerateTurns();
    size_t counter = 0;
    for(auto move : moves){
        Board copy(board);
        copy.ExecuteTurn(move);
        counter += MovesCounter(copy,depth-1);
    }
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

int main()
{
    using namespace std;
    std::string fen;
    std::getline(std::cin,fen);
    size_t depth;
    cin >> depth;
    Board board(fen);
    SeeBoard(board,depth);
    while(true){
        int move;
        cin >> move;
        auto turn = board.GenerateTurns()[move];
        board.ExecuteTurn(turn);
        depth--;
    SeeBoard(board,depth);
    }
    return 0;
}
