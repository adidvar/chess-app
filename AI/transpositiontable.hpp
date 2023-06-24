#ifndef TRANSPOSITIONTABLE_HPP
#define TRANSPOSITIONTABLE_HPP

#include "bitboard.hpp"
#include <position_rating.hpp>
#include <optional>

//template<typename T>
class TransPositionTable{
public:
    struct Element{
        BitBoard board;
        MainAppraiser value = MainAppraiser::Invalid();
        Turn best_turn;
        int depth;
    };

    TransPositionTable(size_t size = 20000000):
        ksize_(size),
        table_(size),
        used_(size)
    {
    }

    std::optional<Element> Find(bitboard_hash_t hash, const BitBoard & board){
        if(used_[hash%ksize_] == false)
            return {};
        if(table_[hash%ksize_].board != board)
            return {};
        else
            return table_[hash%ksize_];
    }
    void Write(Element element, bitboard_hash_t hash){
        table_[hash%ksize_] = element;
        used_[hash%ksize_] = true;
    }

    void Clear(){
        for(size_t i = 0 ; i < ksize_ ; i++)
            used_[i] = false;
    }

private:
    const size_t ksize_;
    std::vector<Element> table_;
    std::vector<bool> used_;
};

#endif
