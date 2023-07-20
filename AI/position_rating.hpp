#ifndef POSITION_RATING_HPP
#define POSITION_RATING_HPP

#include <string>
#include <bitboard.hpp>
#include <magic.hpp>

class MainAppraiser
{

    //material
    constexpr static int pawn = 100;
    constexpr static int knight = 320;
    constexpr static int bishop = 330;
    constexpr static int rook = 500;
    constexpr static int queen = 900;

    //moves counter
    constexpr static int movementbonus = 4;

    //position bonuses

    constexpr static int pawn_bonus[64] {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5, -10,  0,  0, -10, -5,  5,
        5, 10, 10, -20, -20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    };
    constexpr static int knight_bonus[64] {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20,  0,  0,  0,  0, -20, -40,
        -30,  0, 10, 15, 15, 10,  0, -30,
        -30,  5, 15, 20, 20, 15,  5, -30,
        -30,  0, 15, 20, 20, 15,  0, -30,
        -30,  5, 10, 15, 15, 10,  5, -30,
        -40, -20,  0,  5,  5,  0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50,
    };
    constexpr static int bishop_bonus[64] {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10,  0,  0,  0,  0,  0,  0, -10,
        -10,  0,  5, 10, 10,  5,  0, -10,
        -10,  5,  5, 10, 10,  5,  5, -10,
        -10,  0, 10, 10, 10, 10,  0, -10,
        -10, 10, 10, 10, 10, 10, 10, -10,
        -10,  5,  0,  0,  0,  0,  5, -10,
        -20, -10, -10, -10, -10, -10, -10, -20,
    };
    constexpr static int rook_bonus[64] {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    };
    constexpr static int queen_bonus[64] {
        -20, -10, -10, -5, -5, -10, -10, -20,
        -10,  0,  0,  0,  0,  0,  0, -10,
        -10,  0,  5,  5,  5,  5,  0, -10,
        -5,  0,  5,  5,  5,  5,  0, -5,
        0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0, -10,
        -10,  0,  5,  0,  0,  0,  0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20
    };
    constexpr static int king_bonus[64] {
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        20, 20,  0,  0,  0,  0, 20, 20,
        20, 30, 10,  0,  0, 10, 30, 20
    };

    constexpr static int defended_pawn_bonus = 9;
    constexpr static int doubled_pawn_punishment = 11;

    static int CalculateBonus(bitboard_t map, const int bonus[], Color color)
    {
        int value = 0;
        BitIterator i(map);
        for(; i.Valid(); ++i) {
            Position index = log2_64(i.Bit());
            value += bonus[(color == Color::kWhite ? index : index.Rotate()).Value()];
        }
        return value;
    }

    //rangement
    constexpr static int max_depth = 50;
    constexpr static int range = 100000;

    int value_;


public:
    MainAppraiser(int value):
        value_(value) {}

    bool operator < (const MainAppraiser &value) const
    {
        return value_ < value.value_;
    }
    bool operator > (const MainAppraiser &value) const
    {
        return value_ > value.value_;
    }
    bool operator <= (const MainAppraiser &value) const
    {
        return value_ <= value.value_;
    }
    bool operator >= (const MainAppraiser &value) const
    {
        return value_ >= value.value_;
    }
    bool operator == (const MainAppraiser &value) const
    {
        return value_ == value.value_;
    }
    bool operator != (const MainAppraiser &value) const
    {
        return value_ != value.value_;
    }

    static MainAppraiser Invalid()
    {
        return MainAppraiser(-99999999);
    };

    static MainAppraiser CheckMateWin(int depth)
    {
        return MainAppraiser(range - max_depth + depth);
    };
    static MainAppraiser CheckMateLose(int depth)
    {
        return MainAppraiser(-range + max_depth - depth);
    };
    static MainAppraiser Tie()
    {
        return MainAppraiser(0);
    };
    static MainAppraiser Approximate(BitBoard board, Color color)
    {
        int value = 0;

        //pawns
        value += pawn * count_1s(board.GetBitBoard(color, Figure::kPawn));
        value -= pawn * count_1s(board.GetBitBoard(!color, Figure::kPawn));

        auto wmap = board.AttackMask(Color::kWhite) & ~board.GetColorBitBoard(Color::kWhite);
        value += movementbonus * count_1s(wmap);

        auto bmap = board.AttackMask(Color::kBlack) & ~board.GetColorBitBoard(Color::kBlack);
        value -= movementbonus * count_1s(bmap);

        for(int i = 0; i < 8; ++i) {
            auto white_pawns = board.GetBitBoard(Color::kWhite, Figure::kPawn) & rows[i];
            auto black_pawns = board.GetBitBoard(Color::kBlack, Figure::kPawn) & rows[i];
            if(count_1s(white_pawns) > 1)
                value -= doubled_pawn_punishment;
            if(count_1s(black_pawns) > 1)
                value += doubled_pawn_punishment;
        }

        auto wpawns = board.GetBitBoard(Color::kWhite, Figure::kPawn);
        auto wsafe_pawns = (((wpawns << 9) & wpawns) >> 9) | (((wpawns << 7) & wpawns) >> 7);
        value += defended_pawn_bonus * count_1s(wsafe_pawns);

        auto bpawns = board.GetBitBoard(Color::kBlack, Figure::kPawn);
        auto bsafe_pawns = (((bpawns << 9) & bpawns) >> 9) | (((bpawns << 7) & bpawns) >> 7);
        value -= defended_pawn_bonus * count_1s(bsafe_pawns);

        value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kPawn), pawn_bonus, Color::kWhite);
        value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kPawn), pawn_bonus, Color::kBlack);

        value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kKnight), knight_bonus, Color::kWhite);
        value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kKnight), knight_bonus, Color::kBlack);

        value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kBishop), bishop_bonus, Color::kWhite);
        value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kBishop), bishop_bonus, Color::kBlack);

        value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kRook), rook_bonus, Color::kWhite);
        value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kRook), rook_bonus, Color::kBlack);

        value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kQueen), queen_bonus, Color::kWhite);
        value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kQueen), queen_bonus, Color::kBlack);

        value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kKing), king_bonus, Color::kWhite);
        value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kKing), king_bonus, Color::kBlack);

        value += knight * count_1s(board.GetBitBoard(Color::kWhite, Figure::kKnight));
        value -= knight * count_1s(board.GetBitBoard(Color::kBlack, Figure::kKnight));
        value += bishop * count_1s(board.GetBitBoard(Color::kWhite, Figure::kBishop));
        value -= bishop * count_1s(board.GetBitBoard(Color::kBlack, Figure::kBishop));
        value += rook * count_1s(board.GetBitBoard(Color::kWhite, Figure::kRook));
        value -= rook * count_1s(board.GetBitBoard(Color::kBlack, Figure::kRook));
        value += queen * count_1s(board.GetBitBoard(Color::kWhite, Figure::kQueen));
        value -= queen * count_1s(board.GetBitBoard(Color::kBlack, Figure::kQueen));

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
    std::string ToString()
    {
        return std::to_string(value_);
    };

    int operator -(const MainAppraiser &m)
    {
        return value_ - m.value_;
    }
};


#endif
