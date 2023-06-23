#include "openingsbase.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <set>

#include <bitboard.hpp>
#include <match.hpp>

OpeningsBase::OpeningsBase()
{
    std::vector<std::filesystem::path> pathes;
    for( auto && dir : std::filesystem::recursive_directory_iterator("."))
        if(dir.is_regular_file() && dir.path().filename().extension() == ".tsv")
            pathes.push_back(dir.path());

    for(auto path : pathes){
        std::ifstream file(path);
        if(!file.is_open()){
            std::cerr << "Error while oppening file" << std::endl;
            continue;
        }

        std::string line;
        std::getline(file,line);

        while(!file.eof()){
            std::getline(file,line);

            if(file.eof())
                break;

            auto parts = SplitByDelims(line,{'\t'});

            auto turns = ParseTurns(parts[2],BitBoard());

            openings_.push_back(Opening(turns.first,std::string(parts[1])));
        }
    }
}

std::vector<Turn> OpeningsBase::FindNext(const Match &match) const
{
    std::set<Turn> turns;
    for(auto op : openings_)
    {
        size_t common = 0;
        for(size_t i = 0 ; i < match.GetTurns().size(); ++i){
            if(match.GetTurns()[i] == op.GetTurns()[i])
                common++;
            else
                break;
        }

        if(common == match.GetTurns().size())
            turns.insert(op.GetTurns()[common]);
    }
    return std::vector<Turn>(turns.begin(),turns.end());
}
