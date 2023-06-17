#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include <board.hpp>
#include <bitboard.hpp>
#include <magic.hpp>
#include <match.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <algorithm>
#include <filesystem>

std::string LoadFile(std::filesystem::path path){
    std::ifstream t(path);
    if(!t.is_open()){
        std::cerr << "Error while oppening file" << std::endl;
        exit(1);
    }

    size_t length;
    t.seekg(0, std::ios::end);    // go to the end
    length = t.tellg();           // report location (this is the length)
    t.seekg(0, std::ios::beg);    // go back to the beginning
    char *buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
    t.read(buffer, length);       // read the whole file into the buffer

    std::string string(buffer,t.gcount());
    t.close();                    // close file handle
    delete[] buffer;
    return string;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    using namespace std;

    std::vector<std::filesystem::path> pathes;

    for( auto && dir : filesystem::recursive_directory_iterator("matches"))
        if(dir.is_regular_file() && dir.path().filename().extension() == ".pgn")
            pathes.push_back(dir);


    for(size_t i = 0 ; i < pathes.size() ; i++)
    {
        std::cout << pathes[i]<< " ";
        auto text = LoadFile(pathes[i]);
        auto matches = Match::LoadFromPGN(text);
        cout << i+1 <<  '/' <<  pathes.size() << endl;
    }

    return 0;
}
