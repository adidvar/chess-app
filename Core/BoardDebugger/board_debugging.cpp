#include <iostream>
#include <string>
#include <chrono>
#include <board.h>
#include <bitboard.h>
#include "magic.h"

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

uint64_t log2_64(uint64_t value)
{
    for(size_t i = 0 ; i < 64 ; i++){
        if((value >> i) == 1)
            return i;
    }
}

void PrintBoard(uint64_t board){
    for(size_t i = 0 ; i < 64 ; i++){
        if(i%8==0)
            std::cout << std::endl;
        std::cout << (((board >> i ) & 1 )?'1':'0');
    }
    std::cout << std::endl;
}

constexpr static const uint64_t row_a = (1) + (1<<8) + (1<<16) + (1<<24) + (1LL<<32) + (1LL<<40) + (1LL<<48) + (1LL<<56);
constexpr static const uint64_t row_b = row_a << 1;
constexpr static const uint64_t row_c = row_a << 2;
constexpr static const uint64_t row_d = row_a << 3;
constexpr static const uint64_t row_e = row_a << 4;
constexpr static const uint64_t row_f = row_a << 5;
constexpr static const uint64_t row_g = row_a << 6;
constexpr static const uint64_t row_h = row_a << 7;

constexpr static const uint64_t line_8 = (1) + (1<<1) + (1<<2) + (1<<3) + (1LL<<4) + (1LL<<5) + (1LL<<6) + (1LL<<7);
constexpr static const uint64_t line_1 = line_8 << 56;

constexpr static const uint64_t window = ~(row_a|row_h|line_1|line_8);
constexpr static const uint64_t window_v = ~(row_a|row_h);
constexpr static const uint64_t window_h = ~(line_1|line_8);

uint64_t verticals_masks[64];
uint64_t horizontals_masks[64];
// 4 - type of attack 0-horizontal 1-vertical 2 3 - bishops main , additional
// 64 - position
uint64_t horizontals_processed[64][64];
uint64_t verticals_processed[64][64];

//define Magic`s here
//Magic horizontals_magic[8] = {row_a , row_b ,  row_c, row_d , row_e , row_f , row_g , row_h};

//4- attack type
//64 - position
//32 - mask of borders

uint64_t mask_attack(uint64_t position, uint64_t borders){
    uint64_t left = position , right = position;
    uint64_t result = 0;
    while(left){
        left <<= 1;
        result |= left;
        left &= ~borders;
    }
    while(right){
        right >>= 1;
        result |= right;
        right &= ~borders;
    }
    return (result & line_8);
}


void fill_constants(){
    /*
    for(size_t position = 0 ; position < 64 ; ++position)
    {
        uint64_t value = 0;
        for(size_t i = position/8 , j=position%8 ; i<8 && j<8;){
                value |= (1LL<<(8*i+j));
                i++;
                j++;
        }
        for(size_t i = position/8 , j=position%8 ; i<8 && j<8;){
                value |= (1LL<<(8*i+j));
                i--;
                j--;
        }
        attack_masks[2][position] = value;
    }
    for(size_t position = 0 ; position < 64 ; ++position)
    {
        uint64_t value = 0;
        for(size_t i = position/8 , j=position%8 ; i<8 && j<8;){
                value |= (1LL<<(8*i+j));
                i--;
                j++;
        }
        for(size_t i = position/8 , j=position%8 ; i<8 && j<8;){
                value |= (1LL<<(8*i+j));
                i++;
                j--;
        }
        attack_masks[3][position] = value;
    }
    */
    for(size_t position = 0 ; position < 64 ; ++position)
    {
        uint64_t value = 0;
        for(size_t i = position/8 , j=position%8 ; i<8 && j<8;){
                value |= (1LL<<(8*i+j));
                j++;
        }
        for(size_t i = position/8 , j=position%8 ; i<8 && j<8;){
                value |= (1LL<<(8*i+j));
                j--;
        }
        horizontals_masks[position] = value;
    }
    for(size_t position = 0 ; position < 64 ; ++position)
    {
        uint64_t value = 0;
        for(size_t i = position/8 , j=position%8 ; i<8 && j<8;){
                value |= (1LL<<(8*i+j));
                i++;
        }
        for(size_t i = position/8 , j=position%8 ; i<8 && j<8;){
                value |= (1LL<<(8*i+j));
                i--;
        }
        verticals_masks[position] = value;
    }
    // std::cout << "--------------------------------------";
    //horizontals
    for(size_t position = 0 ; position < 64 ;++position){
        for(uint64_t borders_plot = 0 ; borders_plot < 64 ;++borders_plot){
           // PrintBoard(1LL << position);
           // PrintBoard(borders_plot);
            uint64_t attack_plot = mask_attack(1LL << (position%8),borders_plot << 1);
           // PrintBoard(attack_plot);
            uint64_t attack_result = attack_plot << 8*(position/8);
           // PrintBoard(attack_result);
            uint64_t index = borders_plot;
           // PrintBoard(index);
            horizontals_processed[position][index] = attack_result;
        }
    }
    for(size_t position = 0 ; position < 64 ;++position){
        for(uint64_t borders_plot = 0 ; borders_plot < 64 ;++borders_plot){
           // PrintBoard(1LL << position);
           // PrintBoard(borders_plot);
            uint64_t attack_plot = mask_attack(1LL << (position%8),borders_plot << 1);
           // PrintBoard(attack_plot);
            uint64_t attack_result = attack_plot << 8*(position/8);
           // PrintBoard(attack_result);
            uint64_t index = borders_plot;
           // PrintBoard(index);
            horizontals_processed[position][index] = attack_result;
        }
    }


}
uint64_t generate_horizontals(uint64_t position, uint64_t borders)
{
    position = log2_64(position);
    borders >>= 8*(position/8);
    borders >>= 1;
    borders &= ((1<<6)-1);
    std::cout << position << ": " << borders << std::endl;
    return horizontals_processed[position][borders];
}

/*
uint64_t attack_map(uint64_t figure, uint64_t borders,  )
{
    figure = log2_64(figure);

    uint64_t attack_mask = attack_masks[attack_index][figure];
    uint64_t real_borders = borders & window & attack_masks[attack_index][figure];
    //magic trasform of real borders

    return attack_after[attack_index][figure][real_borders];
}
*/

uint64_t read_mask()
{
    uint64_t result = 0;
    for(size_t i = 0 ; i < 8 ; i++)
    {
        std::string line;
        std::getline(std::cin,line);
        for(size_t j = 0; j < 8 ; j++){
            if(line[j] == '1')
                result |= (1LL << (8*j+i));
        }
    }
    return result;
}
/*
uint64_t test_generation(uint64_t position , uint64_t borders_plot){
    auto simple_border_mask = window_v & (~(1LL<<position)) & attack_masks[0][position];
    PrintBoard(attack_masks[0][position] & window_v);
    PrintBoard(simple_border_mask);
    auto borders = SlowMagic(attack_masks[0][position],borders_plot);
    PrintBoard(borders);
    auto index = SlowMagicRev(simple_border_mask,borders);
    return attack_after[0][position][index];
}
*/


int main()
{
    //BenchMaarkBoards();
    using namespace std;
    fill_constants();
    for(size_t i = 56 ; i <= 56 ; i-=8){
        Magic magic(row_a >> i);
        PrintBoard(row_a >> i);
        PrintBoard(magic.MagicNum());
    }
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
