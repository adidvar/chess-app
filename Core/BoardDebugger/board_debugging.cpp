#include <iostream>
#include <string>
#include <chrono>
#include <board.hpp>
#include <bitboard.hpp>

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

uint64_t random_uint64() {
  uint64_t u1, u2, u3, u4;
  u1 = (uint64_t)(rand()) & 0xFFFF; u2 = (uint64_t)(rand()) & 0xFFFF;
  u3 = (uint64_t)(rand()) & 0xFFFF; u4 = (uint64_t)(rand()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

uint64_t random_uint64_fewbits() {
  return random_uint64() & random_uint64() & random_uint64();
}
static uint64_t rmask1(uint64_t sq) {
  uint64_t result = 0;
  int rk = sq/8, fl = sq%8, r, f;
  for(f = fl+1; f <= 6; f++) result |= (1ULL << (f + rk*8));
  for(f = fl-1; f >= 1; f--) result |= (1ULL << (f + rk*8));
  return result;
}

/*
static uint64_t rmask2(uint64_t sq) {
  uint64_t result = 0;
  int rk = sq/8, fl = sq%8, r, f;
  for(r = rk+1; r <= 6; r++) result |= (1ULL << (fl + r*8));
  for(r = rk-1; r >= 1; r--) result |= (1ULL << (fl + r*8));
  return result;
}
*/

static uint64_t rattack1(uint64_t sq, uint64_t borders)
{
    uint64_t rmask = rmask1(sq);
    borders |= ~rmask;
    uint64_t p = 1ULL<<sq;
    uint64_t result = 0;
    while(p){
        p <<= 1;
        result |= p;
        p &= ~borders;
    }
    p = 1ULL<<sq;
    while(p){
        p >>= 1;
        result |= p;
        p &= ~borders;
    }
    return result & ((rmask<<1)|(rmask>>1));
}
/*
static uint64_t rattack2(uint64_t sq, uint64_t borders)
{
    uint64_t rmask = rmask2(sq);
    borders |= ~rmask;
    uint64_t p = 1<<sq;
    uint64_t result = 0;
    while(p){
        p <<= 8;
        result |= p;
        p &= ~borders;
    }
    p = 1<<sq;
    while(p){
        p >>= 8;
        result |= p;
        p &= ~borders;
    }
    return result;
}
*/

int count_1s(uint64_t b) {
  int r;
  for(r = 0; b; r++, b &= b - 1);
  return r;
}

/// from plot to mask
uint64_t Encode(uint64_t mask, uint64_t data)
{
    uint64_t counter = 0;
    uint64_t result = 0;
    while(mask!=0)
    {
        uint64_t bit = mask^((mask-1)&mask);
        uint64_t value = (data >> counter) &1uLL;

        if(value)
            value = 0xFFFFFFFFFFFF;

        result |= value & bit;

        mask &= mask-1;
        counter++;
    }
    return result;
}

int transform(uint64_t b, uint64_t magic, int bits) {
  return (int)((b * magic) >> bits);
}

uint64_t rmagic[64];
uint64_t rookshift = 6;

uint64_t find_magic(uint64_t mask)
{
    using namespace std;
    size_t n = count_1s(mask);
    size_t c = 1<<n;

    uint64_t tests[1024];
    uint64_t used[1024];
    uint64_t attack[1024];

    for(size_t i=0;i<c;i++){
        tests[i] = Encode(mask,i);
        attack[i] = rattack1(0,1);
    }

    while(true){
        size_t magic = random_uint64_fewbits();
        bool flag = true;
        for(size_t i = 0 ; i < c ; i++)
            used[i] = -1;
        for(size_t i = 0 ; i < c && flag;i++){
            auto j = tests[i]*magic >> (64-n);
            if(used[j]==-1)
                used[j] = i;
            else if(used[j]!=i) flag = false;
        }
        if(!flag)return magic;
    }


    return 0;
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
    uint64_t map[64];
    //BenchMaarkBoards();
    using namespace std;
    auto mask =  rmask1(0);
    PrintBoard(mask);
    for(size_t i = 0 ; i<256;i++){
        auto magic = find_magic(mask);
        auto index = i*magic >> (64-count_1s(mask));
        if(map[index] == 0)
            map[index] = rattack1(0,i);
        else if(map[index]!=rattack1(0,i))
        {
            cout << "error" << endl;
            return 0;
        }
    cout << index << endl;
    }





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
/*
    for(size_t i = 0 ; i < 8 ; i++){
        Magic magic((row_a >> 24) << i);
        PrintBoard((row_a >> 24) << i);
        PrintBoard(magic.MagicNum());
        std::cout << read_mask();

    }
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
