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

uint64_t log2_64(uint64_t value)
{
    for(size_t i = 0 ; i < 64 ; i++){
        if((value >> i) == 1)
            return i;
    }
}

constexpr static const uint64_t row_a = (1) + (1<<8) + (1<<16) + (1<<24) + (1LL<<32) + (1LL<<40) + (1LL<<48) + (1LL<<56);
constexpr static const uint64_t row_h = row_a << 7;

constexpr static const uint64_t line_8 = (1) + (1<<1) + (1<<2) + (1<<3) + (1LL<<4) + (1LL<<5) + (1LL<<6) + (1LL<<7);
constexpr static const uint64_t line_1 = line_8 << 56;

constexpr static const uint64_t window = ~(row_a|row_h|line_1|line_8);

uint64_t attack_masks[4][64];
// 4 - type of attack 1-horizontal 2-vertical 3 4 - bishops
// 64 - position
uint64_t attack_after[4][64][32];
//4- attack type
//64 - position
//32 - mask of borders

uint64_t attack_map(uint64_t figure, uint64_t borders, uint64_t attack_index)
{
    figure = log2_64(figure);

    uint64_t attack_mask = attack_masks[attack_index][figure];
    uint64_t real_borders = borders & window & attack_masks[attack_index][figure];
    //magic trasform of real borders

    return attack_after[attack_index][figure][real_borders];
}


int main()
{
    //BenchMaarkBoards();
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
    cout << log2_64(64);
    uint64_t mask = 1 + (1<<9) + (1<<18) + (1<<27) + (1LL<<36);
    uint64_t data = 1 + (1<<9) + (1<<18);
    PrintBoard(mask);
    PrintBoard(data);
    auto magic = FindMagic(mask);
    std::cout << magic.first << ' ' << magic.second << std::endl;
    magic = FindMagicRev(mask);
    std::cout << magic.first << ' ' << magic.second << std::endl;

/*
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
