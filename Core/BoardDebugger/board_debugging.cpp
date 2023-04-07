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


float BenchMarkOnce(){

    using namespace std::chrono;
    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << MovesCounter<Board>(Board(),3);
    auto end = std::chrono::high_resolution_clock::now() - begin;
    auto last_dur = end;

    begin = std::chrono::high_resolution_clock::now();
    std::cout << MovesCounter<BitBoard>(BitBoard(),3);
    end = std::chrono::high_resolution_clock::now() - begin;
    std::cout << std::endl;

    std::cout << "---------------------------------" << std::endl;
    std::cout << "   Board:" << std::chrono::duration_cast<std::chrono::milliseconds>(last_dur).count() << "   <--->  ";
    std::cout << "BitBoard:" << std::chrono::duration_cast<std::chrono::milliseconds>(end).count() << std::endl;
    std::cout << "   Ratio:" << (float)last_dur.count()/end.count() << 'x' << std::endl;
    std::cout << "---------------------------------" << std::endl;
    return (float)last_dur.count()/end.count();
}

void BenchMark(){
   float value = 0;
   size_t tests = 5;
   for(size_t i = 0 ; i < tests ; i++)
       value+=BenchMarkOnce();

    std::cout << "---------------------------------" << std::endl;
    std::cout << "   Average Ratio:" << (float)value/tests << 'x' << std::endl;
    std::cout << "---------------------------------" << std::endl;
}

void PrintBitBoard(uint64_t board){
    const char letters[] = " +";

    using namespace std;
    for(size_t i = 0 ; i<17;i++){
        cout << "-";
    }
    cout << endl;
    for(size_t i = 0 ; i<8;i++){
        cout << '|';
        for(size_t j = 0 ; j<8;j++){
            Position pos(i,j);
            cout << letters[((board >> pos.Value()) & 1) ] << "|";
        }
        cout << endl;
    }
    for(size_t i = 0 ; i<17;i++){
        cout << "-";
    }
    cout << endl;
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

template <class T>
void PrintBoard(const T& board){

    const char letters[2][8] = {" pnbrqk"," PNBRQK"};

    using namespace std;
    for(size_t i = 0 ; i<17;i++){
        cout << "-";
    }
    cout << endl;
    for(size_t i = 0 ; i<8;i++){
        cout << '|';
        for(size_t j = 0 ; j<8;j++){
            Position pos(i,j);
            auto type = board.GetCell(pos);
            cout << letters[type.color][type.type] << "|";
        }
        cout << endl;
    }
    for(size_t i = 0 ; i<17;i++){
        cout << "-";
    }
    cout << endl;

}

bool CompareUntillError(Board board , size_t depth){
    BitBoard bitboard(board.Fen());

    if(board.GenerateSubBoards().size() != bitboard.GenerateSubBoards().size()){
        PrintBoard(board);
        std::cout << board.Fen() << std::endl;
        std::cout << "Bitboard: "  << bitboard.GenerateSubBoards().size() << std::endl;
        std::cout << "Board: "  << board.GenerateSubBoards().size() << std::endl;

        for(auto board : bitboard.GenerateSubBoards()){
            PrintBoard(board);
        }

        return true;
    }
    if(depth==0)
        return false;

    for(auto sub : board.GenerateSubBoards()){
        if(CompareUntillError(sub,depth-1))
            return true;
    }

    return false;
}

int main()
{
    //BenchMaarkBoards();
    using namespace std;
    InitMagic();
    //BenchMark();
    std::cout << BitBoard("8/8/1p3P2/8/3B4/4p3/5p2/8 w - - 0 1").GenerateSubBoards().size();
    for(auto board : BitBoard("8/8/1p3P2/8/3B4/4p3/5p2/8 w - - 0 1").GenerateSubBoards()){
        PrintBoard(board);
    }
    /*
    BitBoard board;
    board = board.GenerateSubBoards()[0];
    std::cout << (int)board.CurrentColor();

    CompareUntillError(Board(),4);
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
