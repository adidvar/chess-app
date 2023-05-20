#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include <board.hpp>
#include <bitboard.hpp>
#include <magic.hpp>
#include <computer.hpp>
#include <match.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>



/*
using Tag = std::pair<std::string,std::string>;

Tag ParseTag(std::string_view tag)
{
    return Tag();
}

struct Tags{
    std::unordered_map<std::string,std::string> tags;
};

Tags ParseTags(std::string_view data){
    Tags tags;

    assert(data.front() == '[');
    assert(data.back() == ']');

    size_t index = 0;

    for(;;){
        auto block_begin = data.find('[',index);
        auto block_end = data.find(']',block_begin);

        if(block_begin == std::string::npos || block_end == std::string::npos)break;

        auto sub_block = data.substr(block_begin,block_end-block_begin+1);

        auto space = sub_block.find(' ',0);
        assert(space != std::string::npos);
        auto tag = sub_block.substr(1,space-1);
        auto value = sub_block.substr(space+2,sub_block.size()-space-4);

        tags.tags[std::string(tag)] = std::string(value);

        index = block_end;
    }

    return tags;
}

void ParseTurns(std::string_view data){

    size_t index = 0;

    for(;;){

        auto block_index = data.find( delim, index);
        auto block_text = data.substr( index, block_index - index);
        index = block_index + delim.size();
        return block_text;

    }
}

std::string_view delim = "\n\n";

std::string_view ReadBlock(std::string_view data, size_t & index)
{
    auto block_index = data.find( delim, index);
    auto block_text = data.substr( index, block_index - index);
    index = block_index + delim.size();
    return block_text;
}

Match ReadMatch(std::string_view data, size_t & index){
    //auto data_index = data.find("\n\n",header_index+1);
    //auto data_text = data.substr(header_index,data_index-header_index-1);

    auto header = ReadBlock(data,index);
    auto content = ReadBlock(data,index);

    auto Tags = ParseTags(header);
    Match match;


    return {};
}
*/
/*
Turns ParseTurns(std::string_view data, size_t & index){
    Turns turns;

    auto line_end = data.find("\n\n",index+1);
    auto sub_line = data.substr(index+1,line_end-index-1);

    std::cout << sub_line << std::endl;

    return turns;
}
*/

Match Parse(std::string text, size_t &position){




    return {};
}

std::string LoadFile(std::string name){
    std::ifstream t("Adams.pgn");
    if(!t.is_open()){
        std::cerr << "Error while oppening file";
        exit(1);
    }
    int length;
    t.seekg(0, std::ios::end);    // go to the end
    length = t.tellg();           // report location (this is the length)
    t.seekg(0, std::ios::beg);    // go back to the beginning
    char *buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
    t.read(buffer, length);       // read the whole file into the buffer
    std::string string(buffer,length);
    t.close();                    // close file handle
    delete[] buffer;
    return string;
}

int main()
{
    //BenchMark();
   //SeeBoard(BitBoard(),6);
    //std::cout << MovesCounter(BitBoard(), 6);
    using namespace std;
    //PrintBitBoard(AttackFrom(1ULL << 35));
    /*
    Computer cmp(Color::kWhite);


    auto board = BitBoard();
    while(!board.End()){
        PrintBoard(board);
        board = cmp.GetTurn(board);

    }

*/
    /*
    auto text = LoadFile("Adam.pgn");
    size_t len = 0;
    ReadMatch(text,len);
    */


    //PrintBoardIndexes();

    //BitBoard bitboard("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1" );
//    for(auto bit : bitboard.GenerateSubBoards())
 //       PrintBoard(bit);
    /*
    start :;

    size_t error_depth=1;


    PrintBoard(bitboard);
    //for(auto board : bitboard.GenerateSubBoards())
        //PrintBoard(board);
    CompareSubs(Board(bitboard.Fen()),bitboard,1);
    while(MovesCounter(Board(bitboard.Fen()),error_depth)==MovesCounter(bitboard,error_depth)){
        error_depth++;
        cout << "Board:----->" << MovesCounter(Board(bitboard.Fen()),error_depth) << endl;
        cout << "BitBoard:-->" << MovesCounter(bitboard,error_depth) << endl;
    }

    cout << "ERROR DEPTH: -->" << error_depth << endl;

    //CompareSubs(Board(string),BitBoard(string),3);
    CompareSubC(Board(bitboard.Fen()),bitboard,error_depth);

    //if(error_depth == 1)
        //PrintInside(bitboard);

    size_t num ;
    cin >> num;
    bitboard = bitboard.GenerateSubBoards()[num];


    goto start;
    */
/*

    cout << (MovesCounter(Board(string),2) == MovesCounter(BitBoard(string),2)) << endl;

    CompareSubCount(Board(string),BitBoard(string),1);

    */
    /*
    while(true){
        std::string bord;
        std::getline(std::cin,bord);
        cout << MovesCounter(Board(bord),1) << endl;
        cout << MovesCounter(BitBoard(bord),1) << endl;
    }
    */
    //BenchMark();
   // for(auto board : BitBoard("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9").GenerateSubBoards()){
  //      PrintBoard(board);
  //  }
    //BitBoard board;
    //board = board.GenerateSubBoards()[0];
    //std::cout << (int)board.CurrentColor();

    //CompareUntillErrorSmart(Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"),3);
    /*
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
