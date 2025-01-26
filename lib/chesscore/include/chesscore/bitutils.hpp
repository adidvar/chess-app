#pragma once

#include <bit>
#include <string_view>

#include "bitasm.hpp"
#include "figures.hpp"

constexpr bitboard getBitBoardOne()
{
    return static_cast<bitboard>(1);
}

constexpr bitboard positionToMask(Position position)
{
  return getBitBoardOne() << position.index();
}

constexpr bitboard operator"" _bm(const char *str, std::size_t len)
{
    return positionToMask(Position(std::string_view(str, len)));
}

constexpr bitboard row_a = (1ULL) + (1ULL << 8ULL) + (1ULL << 16ULL) + (1ULL << 24ULL)
                           + (1ULL << 32ULL) + (1ULL << 40ULL) + (1ULL << 48ULL) + (1ULL << 56ULL);
constexpr bitboard row_b = row_a << 1ULL;
constexpr bitboard row_c = row_a << 2ULL;
constexpr bitboard row_d = row_a << 3ULL;
constexpr bitboard row_e = row_a << 4ULL;
constexpr bitboard row_f = row_a << 5ULL;
constexpr bitboard row_g = row_a << 6ULL;
constexpr bitboard row_h = row_a << 7ULL;
constexpr bitboard rows[8]{row_a, row_b, row_c, row_d, row_e, row_f, row_g, row_h};

constexpr bitboard line_8 = (1ULL) + (1ULL << 1ULL) + (1ULL << 2ULL) + (1ULL << 3ULL)
                            + (1ULL << 4ULL) + (1ULL << 5ULL) + (1ULL << 6ULL) + (1ULL << 7ULL);
constexpr bitboard line_7 = line_8 << 8ULL;
constexpr bitboard line_6 = line_8 << 16ULL;
constexpr bitboard line_5 = line_8 << 24ULL;
constexpr bitboard line_4 = line_8 << 32ULL;
constexpr bitboard line_3 = line_8 << 40ULL;
constexpr bitboard line_2 = line_8 << 48ULL;
constexpr bitboard line_1 = line_8 << 56ULL;
constexpr bitboard lines[8]{line_1, line_2, line_3, line_4, line_5, line_6, line_7, line_8};

inline bitboard takeBit(bitboard &board)
{
    bitboard bit = board & (-board);
    board ^= bit;
    return bit;
}

constexpr int popCount(bitboard b)
{
    return std::popcount(b);
}
