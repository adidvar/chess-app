#include <iostream>
#include <string>
#include <chrono>
#include <board.hpp>
#include <bitboard.hpp>
#include <magic.hpp>

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

void PrintBoard(uint64_t board){
    for(size_t i = 0 ; i < 64 ; i++){
        if(i%8==0)
            std::cout << std::endl;
        std::cout << (((board >> i ) & 1 )?'1':'0');
    }
    std::cout << std::endl;
}

uint64_t read_mask()
{
    uint64_t result = 0;
    for(size_t i = 0 ; i < 8 ; i++)
    {
        std::string line;
        std::getline(std::cin,line);
        for(size_t j = 0; j < 8 ; j++){
            if(line[j] == '1')
                result |= (1LL << (8*i+j));
        }
    }
    return result;
}

int main()
{
    //BenchMaarkBoards();
    using namespace std;
    InitMagic();

    /*
    for(size_t i = 56 ; i >= 24 ; i-=8){
        Magic magic(row_a>>i);
        PrintBoard(magic.MagicNum());
    }
    uint64_t num = read_mask();
    cout <<endl<< num << endl;
    PrintBoard(num);
    PrintBoard((row_a >> 8)*num);
    std::cout << TestMagic(row_a >> 8,num);

    */
    //auto borders = 64;
    //auto position = 16;
    //PrintBoard(borders);
    //PrintBoard(position);
    //PrintBoard(mask_attack(position,borders));
    //PrintBoard(generate_horizontals(position,borders));
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
    cout << read_mask();
    cout << log2_64(64);
    uint64_t mask = 1 + (1<<9) + (1<<18) + (1<<27) + (1LL<<36);
    uint64_t data = 1 + (1<<9) + (1<<18);
    PrintBoard(mask);
    PrintBoard(data);
    auto magic = FindMagic(mask);
    std::cout << magic.first << ' ' << magic.second << std::endl;
    magic = FindMagicRev(mask);
    std::cout << magic.first << ' ' << magic.second << std::endl;
    */
    /*
    fill_constants();
    PrintBoard(attack_masks[1][0] >> 24);
    auto mask = FindMagicRev(attack_masks[1][0] >>24 ).first;
    PrintBoard(mask);
    PrintBoard((attack_masks[1][0] >> 24)*mask);

    //PrintBoard(test_generation(4,32+2));
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
