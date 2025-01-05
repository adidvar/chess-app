#pragma once

#include "bitboard.hpp"
#include <array>
#include <bit>
#include <cinttypes>

using bb = BitBoard::bitboard;

constexpr bb getBitBoardOne()
{
    return static_cast<bb>(1);
}

constexpr bb positionToMask(Position position)
{
    return getBitBoardOne() << position.index();
}

/*
constexpr static bb krooking_masks[2][2]{
    {56_b + 58_b + 59_b + 60_b, 60_b + 61_b + 62_b + 63_b},
    {0_b + 2_b + 3_b + 4_b, 4_b + 5_b + 6_b + 7_b}};
*/

constexpr static const bb row_a = (1ULL) + (1ULL << 8ULL) + (1ULL << 16ULL) + (1ULL << 24ULL)
                                  + (1ULL << 32ULL) + (1ULL << 40ULL) + (1ULL << 48ULL)
                                  + (1ULL << 56ULL);
constexpr static const bb row_b = row_a << 1ULL;
constexpr static const bb row_c = row_a << 2ULL;
constexpr static const bb row_d = row_a << 3ULL;
constexpr static const bb row_e = row_a << 4ULL;
constexpr static const bb row_f = row_a << 5ULL;
constexpr static const bb row_g = row_a << 6ULL;
constexpr static const bb row_h = row_a << 7ULL;

constexpr static const bb rows[8]{row_a, row_b, row_c, row_d, row_e, row_f, row_g, row_h};

constexpr static const bb line_8 = (1ULL) + (1ULL << 1ULL) + (1ULL << 2ULL) + (1ULL << 3ULL)
                                   + (1ULL << 4ULL) + (1ULL << 5ULL) + (1ULL << 6ULL)
                                   + (1ULL << 7ULL);

constexpr static const bb line_7 = line_8 << 8ULL;
constexpr static const bb line_6 = line_8 << 16ULL;
constexpr static const bb line_5 = line_8 << 24ULL;
constexpr static const bb line_4 = line_8 << 32ULL;
constexpr static const bb line_3 = line_8 << 40ULL;
constexpr static const bb line_2 = line_8 << 48ULL;
constexpr static const bb line_1 = line_8 << 56ULL;

constexpr static const bb lines[8]{line_1, line_2, line_3, line_4, line_5, line_6, line_7, line_8};

struct BitIterator {
    constexpr BitIterator(bb value)
        : m_value(value)
    {}

    [[nodiscard]] constexpr bb Value() const { return m_value; }
    [[nodiscard]] constexpr bb Bit() const { return m_value & -m_value; }
    constexpr BitIterator& operator=(bb value)
    {
        m_value = value;
        return *this;
    }
    constexpr void operator++() { m_value &= (m_value - 1); }
    [[nodiscard]] constexpr bool Valid() const { return m_value != 0; }

private:
    bb m_value;
};
#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(_BitScanForward)

inline unsigned log2_64(bb value)
{
    unsigned long result = 0;
    _BitScanForward64(&result, value);
    return result;
}
#else
constexpr const int tab64[64] = {63, 0,  58, 1,  59, 47, 53, 2,  60, 39, 48, 27, 54, 33, 42, 3,
                                 61, 51, 37, 40, 49, 18, 28, 20, 55, 30, 34, 11, 43, 14, 22, 4,
                                 62, 57, 46, 52, 38, 26, 32, 41, 50, 36, 17, 19, 29, 10, 13, 21,
                                 56, 45, 25, 31, 35, 16, 9,  12, 44, 24, 15, 8,  23, 7,  6,  5};

constexpr unsigned log2_64(bb value)
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    return tab64[((bb) ((value - (value >> 1)) * 0x07EDD5E59A4E28C2)) >> 58];
}
#endif

#ifdef _MSC_VER
#include <intrin.h>
inline int popCount(bb b)
{
    return __popcnt64(b);
}
#else
constexpr int popCount(bb b)
{
    return std::popcount(b);
}
#endif
constexpr std::array<BitBoard::bitboard, 64> generateKnightAttacks()
{
    std::array<BitBoard::bitboard, 64> result;

    for (int i = 0; i < 64; i++) {
        BitBoard::bitboard figure = positionToMask(i);
        BitBoard::bitboard attack = (((figure << 10) & ~(row_a | row_b))
                                     | ((figure << 17) & (~row_a))
                                     | (((figure >> 6)) & ~(row_a | row_b))
                                     | ((figure >> 15) & ~(row_a))
                                     | ((figure << 6) & ~(row_g | row_h))
                                     | ((figure << 15) & ~(row_h))
                                     | ((figure >> 10) & ~(row_g | row_h))
                                     | ((figure >> 17) & ~(row_h)));
        result[i] = attack;
    }

    return result;
}

constexpr std::array<BitBoard::bitboard, 64> g_knight_attacks = generateKnightAttacks();

constexpr BitBoard::bitboard processKnight(Position position)
{
    return g_knight_attacks[position.index()];
}

constexpr std::array<BitBoard::bitboard, 64> generateKingAttacks()
{
    std::array<BitBoard::bitboard, 64> result;

    for (int i = 0; i < 64; i++) {
        BitBoard::bitboard figure = positionToMask(i);
        BitBoard::bitboard attack = (((figure << 1) & ~row_a) | ((figure << 9) & ~row_a)
                                     | ((figure >> 7) & ~row_a) | (figure >> 8)
                                     | ((figure >> 1) & ~row_h) | ((figure >> 9) & ~row_h)
                                     | ((figure << 7) & ~row_h) | (figure << 8));
        result[i] = attack;
    }

    return result;
}

constexpr std::array<BitBoard::bitboard, 64> g_king_attacks = generateKingAttacks();

constexpr BitBoard::bitboard processKing(Position position)
{
    return g_king_attacks[position.index()];
}

template<typename Lambda>
constexpr void BitForEach(BitBoard::bitboard bb, Lambda lambda)
{
    while (bb) {
        lambda(bb & -bb);
        bb &= (bb - 1);
    }
}
