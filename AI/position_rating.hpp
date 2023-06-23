#ifndef POSITION_RATING_HPP
#define POSITION_RATING_HPP

#include <string>
#include <bitboard.hpp>
#include <magic.hpp>

class MainAppraiser{

    //material
    constexpr static int pawn = 10;
    constexpr static int knight = 30;
    constexpr static int bishop = 30;
    constexpr static int rook = 50;
    constexpr static int queen = 90;

    //moves counter
    constexpr static int movementbonus = 1;

    //pawns
    constexpr static int running_out_bonus = 2;
    constexpr static int free_row_bonus = 20;
    constexpr static int doubled_pawn_punishment = 4;
    constexpr static int safe_pawn_bonus = 4;

    //rooking and king safety
    constexpr static int safe_king_position = 70;
    constexpr static int rooking_lose_punishment = 30;

    //map controll
    constexpr static int bigcentercontrollpawn = 5;
    constexpr static int bigcentercontroll = 10;

    constexpr static bitboard_t bigcentermask = 66229406269440;

    //rangement
    constexpr static int max_depth = 50;
    constexpr static int range = 1000;

    int value_;

    MainAppraiser(int value):
        value_(value){}

public:

    bool operator < (const MainAppraiser &value) const {
        return value_ < value.value_;
    }
    bool operator > (const MainAppraiser &value) const {
        return value_ > value.value_;
    }
    bool operator <= (const MainAppraiser &value) const {
        return value_ <= value.value_;
    }
    bool operator >= (const MainAppraiser &value) const {
        return value_ >= value.value_;
    }
    bool operator == (const MainAppraiser &value) const {
        return value_ == value.value_;
    }
    bool operator != (const MainAppraiser &value) const {
        return value_ != value.value_;
    }

    static MainAppraiser Invalid()
    {
        return MainAppraiser(-99999999);
    };

    static MainAppraiser CheckMateWin(int depth)
    {
        return MainAppraiser(range-max_depth+depth);
    };
    static MainAppraiser CheckMateLose(int depth)
    {
        return MainAppraiser(-range+max_depth-depth);
    };
    static MainAppraiser Tie(){
        return MainAppraiser(0);
    };
    static MainAppraiser Approximate(BitBoard board, Color color)
    {

        int value = 0;

        //pawns
        value += pawn*count_1s(board.GetBitBoard(color,Figure::kPawn));
        value -= pawn*count_1s(board.GetBitBoard(!color,Figure::kPawn));

        {
            BitIterator iterator(board.GetBitBoard(Color::kWhite,Figure::kPawn));
            for(;iterator.Valid();++iterator)
            {
                auto y = Position(log2_64(iterator.Bit())).y();
                value += running_out_bonus*(6-y);
            }
        }
        {
            BitIterator iterator(board.GetBitBoard(Color::kBlack,Figure::kPawn));
            for(;iterator.Valid();++iterator)
            {
                auto y = Position(log2_64(iterator.Bit())).y();
                value -= running_out_bonus*(y-1);
            }
        }

        for(int i = 0; i < 8; ++i)
        {
            auto white_pawns = board.GetBitBoard(Color::kWhite,Figure::kPawn) & rows[i];
            auto black_pawns = board.GetBitBoard(Color::kBlack,Figure::kPawn) & rows[i];
            if(count_1s(white_pawns) > 1)
                value -= doubled_pawn_punishment;
            if(count_1s(black_pawns) > 1)
                value += doubled_pawn_punishment;
        }

        auto wpawns = board.GetBitBoard(Color::kWhite,Figure::kPawn);
        auto wsafe_pawns = (((wpawns << 9) & wpawns) >> 9) | (((wpawns << 7) & wpawns) >> 7);
        value += safe_pawn_bonus*count_1s(wsafe_pawns);

        auto bpawns = board.GetBitBoard(Color::kBlack,Figure::kPawn);
        auto bsafe_pawns = (((bpawns << 9) & bpawns) >> 9) | (((bpawns << 7) & bpawns) >> 7);
        value -= safe_pawn_bonus*count_1s(bsafe_pawns);

        for(size_t i = 0 ; i < 8 ; ++i){
            auto wpawns = board.GetBitBoard(Color::kWhite,Figure::kPawn) & rows[i];
            auto bpawns = board.GetBitBoard(Color::kBlack,Figure::kPawn) & rows[i];

            auto mask = rows[i];
            if(i != 0)
                mask |= rows[i-1];
            if(i != 7)
                mask |= rows[i+1];

            if( (mask & bpawns) == 0){
                 auto figures = wpawns & rows[i];
                 value += free_row_bonus*count_1s(figures);
            }

            if( (mask & wpawns) == 0){
                 auto figures = bpawns & rows[i];
                 value -= free_row_bonus*count_1s(figures);
            }
        }

        auto wmap = board.AttackMask(Color::kWhite) & ~board.GetColorBitBoard(Color::kWhite);
        value += movementbonus * count_1s(wmap);

        auto bmap = board.AttackMask(Color::kBlack) & ~board.GetColorBitBoard(Color::kBlack);
        value -= movementbonus * count_1s(bmap);

        value += bigcentercontrollpawn*count_1s(board.GetBitBoard(Color::kWhite,Figure::kPawn) & bigcentermask);
        value -= bigcentercontrollpawn*count_1s(board.GetBitBoard(Color::kBlack,Figure::kPawn) & bigcentermask);

        value += bigcentercontroll*count_1s(board.GetColorBitBoard(Color::kWhite) & ~board.GetBitBoard(Color::kWhite,Figure::kPawn) & bigcentermask);
        value -= bigcentercontroll*count_1s(board.GetColorBitBoard(Color::kBlack) & ~board.GetBitBoard(Color::kBlack,Figure::kPawn) & bigcentermask);

        if(board.Test(Figure::kKing,Position(60)) && !board.RookingFlags().white_oo)
            value-=rooking_lose_punishment;
        if(board.Test(Figure::kKing,Position(60)) && !board.RookingFlags().white_ooo)
            value-=rooking_lose_punishment;

        if(board.Test(Figure::kKing,Position(4)) && !board.RookingFlags().black_oo)
            value+=rooking_lose_punishment;
        if(board.Test(Figure::kKing,Position(4)) && !board.RookingFlags().black_ooo)
            value+=rooking_lose_punishment;

        if((board.GetBitBoard(Color::kWhite,Figure::kKing) & 13835058055282163712ull) !=0   && count_1s(board.GetColorBitBoard(Color::kWhite) & 63050394783186944) == 3)
            value+=safe_king_position;
        if((board.GetBitBoard(Color::kBlack,Figure::kKing) & 192ull) !=0   && count_1s(board.GetColorBitBoard(Color::kBlack) & 57344) == 3)
            value-=safe_king_position;

        if((board.GetBitBoard(Color::kWhite,Figure::kKing) & 504403158265495552) !=0   && count_1s(board.GetColorBitBoard(Color::kWhite) & 1970324836974592) == 3)
            value+=safe_king_position;
        if((board.GetBitBoard(Color::kBlack,Figure::kKing) & 7ull) !=0   && count_1s(board.GetColorBitBoard(Color::kBlack) & 1792) == 3)
            value-=safe_king_position;

        value += knight*count_1s(board.GetBitBoard(Color::kWhite,Figure::kKnight));
        value -= knight*count_1s(board.GetBitBoard(Color::kBlack,Figure::kKnight));
        value += bishop*count_1s(board.GetBitBoard(Color::kWhite,Figure::kBishop));
        value -= bishop*count_1s(board.GetBitBoard(Color::kBlack,Figure::kBishop));
        value += rook*count_1s(board.GetBitBoard(Color::kWhite,Figure::kRook));
        value -= rook*count_1s(board.GetBitBoard(Color::kBlack,Figure::kRook));
        value += queen*count_1s(board.GetBitBoard(Color::kWhite,Figure::kQueen));
        value -= queen*count_1s(board.GetBitBoard(Color::kBlack,Figure::kQueen));

        if(color == Color::kBlack)
            value = -value;

        return MainAppraiser{value};
    };
    static MainAppraiser Max()
    {
        return MainAppraiser(range);
    };
    static MainAppraiser Min()
    {
        return MainAppraiser(-range);
    };
    std::string ToString(size_t depth)
    {
        return std::to_string(value_);
    };
};


#endif
