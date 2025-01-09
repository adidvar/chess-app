#pragma once

#include "bitutils.hpp"

#include <bit>
#include <random>

using bitboard = uint64_t;

class Xorshift64
{
private:
    uint64_t state;

public:
    // Constructor initializes the PRNG state
    constexpr Xorshift64(uint64_t seed)
        : state(seed)
    {}

    // Generates a pseudo-random 64-bit number
    //uint64_t next() { return state(); }

    //bitboard few_bits_next() { return next() & next() & next(); }
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
    Xorshift64 rand;
    constexpr static std::array<bitboard, 128> precalculated_magic{
        9331458702791954561,  18014690835701760,    72075221663744256,   1188959099868286976,
        4971982784777826560,  5476553077400012816,  900792493384139008,  4935945917447685760,
        140808374272000,      20407210741858432,    140874935734272,     5188287680018907264,
        1688867580152320,     2342012552319337472,  4684447308505426176, 18295874595684608,
        18016047781676066,    9009673424355429,     4715805656752128,    118360777731082240,
        180162676926775300,   216455356669274130,   2359978563987441665, 4629711412057673860,
        324268253805289476,   2380328327081639936,  653059406677803136,  4613596978228514880,
        1152925904809232384,  4612251169559938048,  2314868539390427649, 4406636464257,
        3098429169344640,     292734250661191688,   1161933239632281858, 5189554283204325377,
        873988600935482368,   141089684065281,      54184044694339840,   429530284161,
        594477908723073024,   580965726857216000,   1159819841635483696, 52789445197832,
        11343661598048272,    563023370649636,      6953566629360042000, 73254139782103045,
        1407651908986112,     2324631468282773760,  1171503320381014528, 721789820677947520,
        4901042844321972608,  288934080775848320,   5188287654248120448, 4657025499792589312,
        23925650045801515,    4612005701568495745,  11540751607664649,   576465873531850753,
        75716803104347669,    8162808751456773,     1154065551020541444, 554655352898,
        20301419236165664,    2310984330215751696,  3461018529841087488, 63226868689633316,
        36611606901690368,    576900574671306753,   9148692793664642,    576742399112905760,
        2882308229692924032,  1188959166723653664,  5669636038328832,    10450607350715793408,
        1130315670097992,     4646546071364048,     4611686328336648704, 216736834129037442,
        1154047440495052321,  1407546816496644,     4756931513180754180, 9367773201101291520,
        1409608803944977,     563135039799808,      4611826774306459654, 35185513076737,
        5770241569078642688,  4900778412250105344,  566258286723680,     577587751723993216,
        166914955545690113,   4504701288876041,     9262778812776072208, 10395293239837197344,
        13839579315954132501, 2315414343885260816,  4621010495649351840, 5296797212326756928,
        586594951245332544,   72356749247578176,    9512202900980498568, 9297683630805581908,
        2380155738645071920,  565291247478016,      72571257618688,      9295429906043243010,
        4755805673439101952,  1157460290932056322,  144724334714291240,  2261712600608896,
        4638940782786512234,  9370304178572296192,  14645658226131456,   2887317554974556688,
        432416210307645440,   4707552919616,        578730487980294144,  326528582396563460,
        37417789247490,       11673897943064643584, 1688858467017216,    9225659570905687041,
        1605533271519986688,  1831821005357576,     1143509944149008,    2310399454299570500};

    constexpr bitboard generateMagic(int &index)
    {
        if (index < 128)
            return precalculated_magic[index++];
        else
            //  return rand.few_bits_next();
            return 0;
    }

    constexpr unsigned processRookIndex(uint8_t pos, bitboard borders) const
    {
        return ((borders & rook_masks[pos]) * rook_magic[pos]) >> rook_shifts[pos];
    }

    constexpr unsigned processBishopIndex(uint8_t pos, bitboard borders) const
    {
        return ((borders & bishop_masks[pos]) * bishop_magic[pos]) >> bishop_shifts[pos];
    }

    constexpr MagicConsts(uint64_t seed)
        : rand(seed)
    {
        for (int position = 0; position < 64; position++) {
            rook_masks[position] = generateFrameLess(position, generateRookMask(position));
            bishop_masks[position] = generateFrameLess(position, generateBishopMask(position));
            rook_shifts[position] = 64 - popCount(rook_masks[position]);
            bishop_shifts[position] = 64 - popCount(bishop_masks[position]);

            int random_index = 0;
            while (true) {
                rook_magic[position] = generateMagic(random_index);
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

            random_index = 0;
            while (true) {
                //random
                bishop_magic[position] = generateMagic(random_index);
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

const MagicConsts g_magic_consts{0};

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
