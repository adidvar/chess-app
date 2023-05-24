#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include <board.hpp>
#include <bitboard.hpp>
#include <magic.hpp>
//#include <computer.hpp>
#include <match.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <algorithm>



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

    //assert(data.front() == '[');
    //assert(data.back() == ']');

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

Turn ParseTurn(std::string_view data, const BitBoard& board){

    const static std::map<char,Figure> bindf{
        {'N',Figure::kKnight},
        {'B',Figure::kBishop} ,
        {'R',Figure::kRook} ,
        {'Q',Figure::kQueen} ,
        {'K',Figure::kKing}
    };

    const static std::map<char,int> bindpx{
        {'a',0},
        {'b',1},
        {'c',2},
        {'d',3},
        {'e',4},
        {'f',5},
        {'g',6},
        {'h',7}
    };

    const static std::map<char,int> bindpy{
        {'1',7},
        {'2',6},
        {'3',5},
        {'4',4},
        {'5',3},
        {'6',2},
        {'7',1},
        {'8',0}
    };

    int from_x = -1;
    int from_y = -1;

    Figure figure = Figure::kPawn;
    Figure figure_transform = Figure::kEmpty;

    if(data.size()!=0 && data.back() == '+'){
        data.remove_suffix(1);
    }

    if(data == "O-O")
        return Turn::GetShortCastling(board.CurrentColor());
    if(data == "O-O-O")
        return Turn::GetLongCastling(board.CurrentColor());

    auto pturns = board.GenerateTurns();

    if(data.size()>=2 && data[data.size()-2] == '='){
        figure_transform = bindf.at(data.back());
        data.remove_suffix(2);
    }

    auto pos = Position::FromString(data.substr(data.size()-2));
    data.remove_suffix(2);


    if(data.size()!=0 && data.back() == 'x'){
        data.remove_suffix(1);
    }

    if(data.size()!=0 && bindpy.count(data.back()) == 1){
        from_y = bindpy.at(data.back());
        data.remove_suffix(1);
    }

    if(data.size()!=0 && bindpx.count(data.back()) == 1){
        from_x = bindpx.at(data.back());
        data.remove_suffix(1);
    }

    if(data.size()!=0 && bindf.count(data.back()) == 1){
        figure = bindf.at(data.back());
        data.remove_suffix(1);
    }

    auto predicate = [&](Turn turn)
    {
        return (turn.to() == pos) && board.Test(figure,turn.from())
                && (from_x == -1 || turn.from().x() == from_x)
                && (from_y == -1 || turn.from().y() == from_y)
                && (figure_transform == turn.figure());
    };

    auto result = std::find_if(pturns.begin(),pturns.end(),predicate);
    auto count = std::count_if(pturns.begin(),pturns.end(),predicate);

    if(count == 1)
        return *result;
    else
        return Turn();
}

std::vector<Turn> ParseTurns(std::string_view data, BitBoard start_pos){
    size_t index = 0;

    std::vector<size_t> split_p;
    std::vector<std::string_view> turns_pre;
    std::vector<std::string_view> turns;
    std::vector<Turn> result;

    split_p.push_back(0);

    for(size_t pos = data.find(".", 0); pos != std::string_view::npos; pos = data.find(".",pos+1))
        split_p.push_back(pos+1);

    for(size_t pos = data.find("\n", 0); pos != std::string_view::npos; pos = data.find("\n",pos+1))
        split_p.push_back(pos+1);

    for(size_t pos = data.find(" ", 0); pos != std::string_view::npos; pos = data.find(" ",pos+1))
        split_p.push_back(pos+1);

    std::sort(split_p.begin(),split_p.end());

    for(size_t i = 0 ; i < split_p.size() - 1 ; i++)
        turns_pre.push_back(data.substr(split_p[i],split_p[i+1]-split_p[i]-1));
    turns_pre.pop_back();

    auto match_result = data.substr(split_p.back());
    for(size_t i = 0 ; i < turns_pre.size(); i++)
        if(i%3!=0)
            turns.push_back(turns_pre[i]);

    BitBoard board(start_pos);
    for(auto turn_data : turns){
        auto turn  = ParseTurn(turn_data, board);
        if(!board.ExecuteTurn(turn))
            exit(-1);
        else
            result.push_back(turn);
    }
    return result;
}

std::string_view delim = "\n\n";

std::string_view ReadBlock(std::string_view data, size_t & index)
{
    auto block_index = data.find( delim, index);
    auto block_text = data.substr( index, block_index - index);
    if(block_index != std::string_view::npos)
        index = block_index + delim.size();
    else
        index = block_index;


    return block_text;
}

Match ReadMatch(std::string_view data, size_t & index){
    //auto data_index = data.find("\n\n",header_index+1);
    //auto data_text = data.substr(header_index,data_index-header_index-1);

    auto header = ReadBlock(data,index);
    auto content = ReadBlock(data,index);

    //try{
    auto tags = ParseTags(header);
    auto turns = ParseTurns(content,BitBoard());
    //} catch (...){
        //std::cout << header << std::endl << content << std::endl;
       // exit(-1);
    //}

    static int i = 1;

    std::cout << i << std::endl;
    i++;


    return {};
}

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
    std::ifstream t(name);
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
    std::ios_base::sync_with_stdio(false);
    using namespace std;

    auto text = LoadFile("Adams.pgn");
    size_t len = 0;
    while(len != std::string_view::npos)
        ReadMatch(text,len);

    std::cout << "OK" << std::endl;

    return 0;
}
