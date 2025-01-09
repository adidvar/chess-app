#pragma once

#include "bitasm.hpp"

#include <array>
#include <bit>
#include <cinttypes>

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

constexpr static const bitboard row_a = (1ULL) + (1ULL << 8ULL) + (1ULL << 16ULL) + (1ULL << 24ULL)
                                        + (1ULL << 32ULL) + (1ULL << 40ULL) + (1ULL << 48ULL)
                                        + (1ULL << 56ULL);
constexpr static const bitboard row_b = row_a << 1ULL;
constexpr static const bitboard row_c = row_a << 2ULL;
constexpr static const bitboard row_d = row_a << 3ULL;
constexpr static const bitboard row_e = row_a << 4ULL;
constexpr static const bitboard row_f = row_a << 5ULL;
constexpr static const bitboard row_g = row_a << 6ULL;
constexpr static const bitboard row_h = row_a << 7ULL;

constexpr static const bitboard rows[8]{row_a, row_b, row_c, row_d, row_e, row_f, row_g, row_h};

constexpr static const bitboard line_8 = (1ULL) + (1ULL << 1ULL) + (1ULL << 2ULL) + (1ULL << 3ULL)
                                         + (1ULL << 4ULL) + (1ULL << 5ULL) + (1ULL << 6ULL)
                                         + (1ULL << 7ULL);

constexpr static const bitboard line_7 = line_8 << 8ULL;
constexpr static const bitboard line_6 = line_8 << 16ULL;
constexpr static const bitboard line_5 = line_8 << 24ULL;
constexpr static const bitboard line_4 = line_8 << 32ULL;
constexpr static const bitboard line_3 = line_8 << 40ULL;
constexpr static const bitboard line_2 = line_8 << 48ULL;
constexpr static const bitboard line_1 = line_8 << 56ULL;

constexpr static const bitboard
    lines[8]{line_1, line_2, line_3, line_4, line_5, line_6, line_7, line_8};

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
constexpr std::array<bitboard, 64> generateKnightAttacks()
{
    std::array<bitboard, 64> result;

    for (int i = 0; i < 64; i++) {
        bitboard figure = positionToMask(i);
        bitboard attack = (((figure << 10) & ~(row_a | row_b)) | ((figure << 17) & (~row_a))
                           | (((figure >> 6)) & ~(row_a | row_b)) | ((figure >> 15) & ~(row_a))
                           | ((figure << 6) & ~(row_g | row_h)) | ((figure << 15) & ~(row_h))
                           | ((figure >> 10) & ~(row_g | row_h)) | ((figure >> 17) & ~(row_h)));
        result[i] = attack;
    }

    return result;
}

constexpr auto g_knight_attacks = generateKnightAttacks();

constexpr bitboard processKnight(Position position)
{
    return g_knight_attacks[position.index()];
}

constexpr std::array<bitboard, 64> generateKingAttacks()
{
    std::array<bitboard, 64> result;

    for (int i = 0; i < 64; i++) {
        bitboard figure = positionToMask(i);
        bitboard attack = (((figure << 1) & ~row_a) | ((figure << 9) & ~row_a)
                           | ((figure >> 7) & ~row_a) | (figure >> 8) | ((figure >> 1) & ~row_h)
                           | ((figure >> 9) & ~row_h) | ((figure << 7) & ~row_h) | (figure << 8));
        result[i] = attack;
    }

    return result;
}

constexpr auto g_king_attacks = generateKingAttacks();

constexpr bitboard processKing(Position position)
{
    return g_king_attacks[position.index()];
}

constexpr std::array<std::array<bitboard, 64>, 64> generateWays()
{
    std::array<std::array<bitboard, 64>, 64> result{};

    constexpr auto min = [](int a, int b) { return (a < b) ? a : b; };
    constexpr auto max = [](int a, int b) { return (a > b) ? a : b; };
    constexpr auto abs = [](int x) { return (x < 0) ? -x : x; };

    constexpr auto computeBetween = [&](int from, int to) {
        bitboard between = 0;
        int fromRow = from / 8, fromCol = from % 8;
        int toRow = to / 8, toCol = to % 8;

        if (fromRow == toRow) { // Same row (horizontal)
            for (int col = min(fromCol, toCol) + 1; col < max(fromCol, toCol); ++col) {
                between |= bitboard(1) << (fromRow * 8 + col);
            }
        } else if (fromCol == toCol) { // Same column (vertical)
            for (int row = min(fromRow, toRow) + 1; row < max(fromRow, toRow); ++row) {
                between |= bitboard(1) << (row * 8 + fromCol);
            }
        } else if (abs(fromRow - toRow) == abs(fromCol - toCol)) { // Same diagonal
            int rowStep = (toRow > fromRow) ? 1 : -1;
            int colStep = (toCol > fromCol) ? 1 : -1;
            for (int step = 1; step < abs(toRow - fromRow); ++step) {
                between |= bitboard(1)
                           << ((fromRow + step * rowStep) * 8 + (fromCol + step * colStep));
            }
        }

        return between;
    };

    for (int from = 0; from < 64; ++from) {
        for (int to = 0; to < 64; ++to) {
            result[from][to] = (from == to) ? 0 : computeBetween(from, to);
        }
    }

    return result;
}

constexpr auto g_ways = generateWays();

constexpr bitboard processWay(Position from, Position to)
{
    return g_ways[from.index()][to.index()];
}
