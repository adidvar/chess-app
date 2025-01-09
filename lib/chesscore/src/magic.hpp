#pragma once

#include "bitutils.hpp"

#include <bit>
#include <random>

using bitboard = uint64_t;

class Xorshift64
{
private:
    std::mt19937_64 state;

public:
    // Constructor initializes the PRNG state
    Xorshift64(uint64_t seed)
        : state(seed)
    {}

    // Generates a pseudo-random 64-bit number
    uint64_t next() { return state(); }

    bitboard few_bits_next() { return next() & next() & next(); }
};

constexpr bitboard generateRookMask(bitboard sq)
{
    bitboard result = 0ULL;
    int rank = sq / 8, file = sq % 8;

    for (int f = file + 1; f <= 7; f++)
        result |= (1ULL << (f + rank * 8));
    for (int f = file - 1; f >= 0; f--)
        result |= (1ULL << (f + rank * 8));

    for (int r = rank + 1; r <= 7; r++)
        result |= (1ULL << (file + r * 8));
    for (int r = rank - 1; r >= 0; r--)
        result |= (1ULL << (file + r * 8));

    return result;
}

constexpr bitboard generateBishopMask(bitboard sq)
{
    bitboard result = 0ULL;
    int rank = sq / 8, file = sq % 8;

    for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++)
        result |= (1ULL << (f + r * 8));
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
        result |= (1ULL << (f + r * 8));
    for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--)
        result |= (1ULL << (f + r * 8));
    for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++)
        result |= (1ULL << (f + r * 8));

    return result;
}

constexpr bitboard generateRookAttack(bitboard sq, bitboard blockers)
{
    bitboard result = 0ULL, p;
    int rk = sq / 8, fl = sq % 8;

    p = 1ULL << sq;
    while ((fl < 7) && !(blockers & (p <<= 1))) {
        result |= p;
        ++fl;
    }
    if (fl < 7)
        result |= p;

    fl = sq % 8;
    p = 1ULL << sq;
    while ((fl > 0) && !(blockers & (p >>= 1))) {
        result |= p;
        --fl;
    }
    if (fl > 0)
        result |= p;

    p = 1ULL << sq;
    int r = rk;
    while ((r < 7) && !(blockers & (p <<= 8))) {
        result |= p;
        ++r;
    }
    if (r < 7)
        result |= p;

    p = 1ULL << sq;
    r = rk;
    while ((r > 0) && !(blockers & (p >>= 8))) {
        result |= p;
        --r;
    }
    if (r > 0)
        result |= p;

    return result;
}

constexpr bitboard generateBishopAttack(bitboard sq, bitboard blockers)
{
    bitboard result = 0ULL;
    int rank = sq / 8, file = sq % 8;

    for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++) {
        bitboard pos = 1ULL << (f + r * 8);
        result |= pos;
        if (blockers & pos)
            break;
    }
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
        bitboard pos = 1ULL << (f + r * 8);
        result |= pos;
        if (blockers & pos)
            break;
    }
    for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--) {
        bitboard pos = 1ULL << (f + r * 8);
        result |= pos;
        if (blockers & pos)
            break;
    }
    for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++) {
        bitboard pos = 1ULL << (f + r * 8);
        result |= pos;
        if (blockers & pos)
            break;
    }

    return result;
}

constexpr bitboard generateFrameLess(uint8_t pos, bitboard board)
{
    auto from = positionToMask(pos);
    if ((from & line_8) == 0)
        board &= ~line_8;
    if ((from & line_1) == 0)
        board &= ~line_1;
    if ((from & row_a) == 0)
        board &= ~row_a;
    if ((from & row_h) == 0)
        board &= ~row_h;
    return board;
}

constexpr bitboard software_pext64(bitboard src, bitboard mask)
{
    bitboard result = 0;
    for (bitboard bit = 1, pos = 0; mask; mask >>= 1, bit <<= 1) {
        if (mask & 1) {
            if (src & bit) {
                result |= 1ULL << pos;
            }
            pos++;
        }
    }
    return result;
}

constexpr uint64_t reverse_pext(uint64_t extracted, uint64_t mask)
{
    uint64_t result = 0;
    uint64_t bit_pos = 0;

    for (uint64_t bit = 0; bit < 64; ++bit) {
        if ((mask >> bit) & 1) {
            result |= ((extracted >> bit_pos) & 1) << bit;
            bit_pos++;
        }
    }

    return result;
}

struct MagicConsts
{
    constexpr unsigned processRookIndex(uint8_t pos, bitboard borders) const
    {
        return ((borders & rook_masks[pos]) * rook_magic[pos]) >> rook_shifts[pos];
    }

    constexpr unsigned processBishopIndex(uint8_t pos, bitboard borders) const
    {
        return ((borders & bishop_masks[pos]) * bishop_magic[pos]) >> bishop_shifts[pos];
    }

    constexpr MagicConsts() {}

    constexpr MagicConsts(uint64_t seed) { init(seed); }

    constexpr void init(uint64_t seed)
    {
        Xorshift64 rand{seed};

        for (int position = 0; position < 64; position++) {
            rook_masks[position] = generateFrameLess(position, generateRookMask(position));
            bishop_masks[position] = generateFrameLess(position, generateBishopMask(position));
            rook_shifts[position] = 64 - popCount(rook_masks[position]);
            bishop_shifts[position] = 64 - popCount(bishop_masks[position]);

            while (true) {
                //random
                rook_magic[position] = rand.few_bits_next();
                int tests = 1 << popCount(rook_masks[position]);
                //generation
                for (uint64_t i = 0; i < tests; i++)
                    rook_results[position][i] = 0;

                bool correct = true;
                for (uint64_t borders = 0; borders < tests; borders++) {
                    uint64_t full_borders = reverse_pext(borders, rook_masks[position]);
                    unsigned magic_index = processRookIndex(position, full_borders);
                    uint64_t result = generateRookAttack(position, full_borders);

                    if (rook_results[position][magic_index] == 0) {
                        rook_results[position][magic_index] = result;
                    } else {
                        correct = (rook_results[position][magic_index] == result);
                        if (!correct)
                            break;
                    }
                }

                if (correct)
                    break;
            }

            while (true) {
                //random
                bishop_magic[position] = rand.few_bits_next();
                int tests = 1 << popCount(bishop_masks[position]);
                //generation
                for (uint64_t i = 0; i < tests; i++)
                    bishop_results[position][i] = 0;

                bool correct = true;
                for (uint64_t borders = 0; borders < tests; borders++) {
                    uint64_t full_borders = reverse_pext(borders, bishop_masks[position]);
                    unsigned magic_index = processBishopIndex(position, full_borders);
                    uint64_t result = generateBishopAttack(position, full_borders);

                    if (bishop_results[position][magic_index] == 0) {
                        bishop_results[position][magic_index] = result;
                    } else {
                        correct = (bishop_results[position][magic_index] == result);
                        if (!correct)
                            break;
                    }
                }
                if (correct)
                    break;
            }

            /*
            //magic generation
            while (true) {
                //random
                bishop_magic[position] = rand.few_bits_next();
                //generation
                for (uint64_t borders = 0; borders < 512; borders++) {
                    bishop_results[position][processBishopIndex(position, borders)]
                        = generateBishopAttack(position, borders);
                }
                //tests
                int correct = 0;
                for (uint64_t borders = 0; borders < 512; borders++) {
                    if (bishop_results[position][processBishopIndex(position, borders)]
                        == generateBishopAttack(position, borders))
                        correct++;
                }
                if (correct == 512)
                    break;
        }
*/
        }
    }

    std::array<bitboard, 64> rook_masks{};
    std::array<uint8_t, 64> rook_shifts{};
    std::array<bitboard, 64> rook_magic{};
    std::array<std::array<bitboard, 4096>, 64> rook_results{};

    std::array<bitboard, 64> bishop_masks{};
    std::array<uint8_t, 64> bishop_shifts{};
    std::array<bitboard, 64> bishop_magic{};
    std::array<std::array<bitboard, 512>, 64> bishop_results{};
};

/*
constexpr MagicConsts generateMagic(uint64_t random_seed)
{
    MagicConsts c;

*/

/*
    using namespace std;
    size_t n = popcount(mask);
    size_t c = 1 << n;

    uint64_t input[1024];
    uint64_t output[1024];

    uint64_t hash[1024];

    // fill associates
    for (size_t i = 0; i < c; i++) {
        input[i] = value_to_int64(mask, i);
        output[i] = attack(sq, input[i]);
    }

    for (size_t k = 0; k < 10000000; k++) {
        size_t magic = random_uint64_fewbits(random);
        bool flag = true;
        // clear hash map
        for (size_t i = 0; i < c; i++)
            hash[i] = 0;

        if (popcount(mask * magic >> (64ULL - n)) < n)
            continue;

        for (size_t i = 0; i < c; i++) {
            uint64_t index = input[i] * magic >> (64 - n);
            // add to hash
            if (hash[index] == 0)
                hash[index] = output[i];
            // hash miss
            else if (hash[index] != output[i]) {
                flag = false;
                break;
            }
        }
        if (flag) {
            for (size_t i = 0; i < c; i++)
                hash_ref[sq][i] = hash[i];

            return magic;
        }
    }
    throw std::runtime_error("can't find magic");
return c;
}
*/

const MagicConsts g_magic_consts;

inline bitboard processRook(Position pos, bitboard borders)
{
    return g_magic_consts
        .rook_results[pos.index()][g_magic_consts.processRookIndex(pos.index(), borders)];
}

inline bitboard processBishop(Position pos, bitboard borders)
{
    return g_magic_consts
        .bishop_results[pos.index()][g_magic_consts.processBishopIndex(pos.index(), borders)];
}
