#pragma once

#include <bit>
#include <random>

#include "bitutils.hpp"
#include "figures.hpp"

using bitboard = uint64_t;

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

    constexpr auto computeBetween = [&](int from, int to) {
        bitboard between = 0;
        int fromRow = from / 8, fromCol = from % 8;
        int toRow = to / 8, toCol = to % 8;

        if (fromRow == toRow) { // Same row (horizontal)
          for (int col = std::min(fromCol, toCol) + 1;
               col < std::max(fromCol, toCol); ++col) {
            between |= bitboard(1) << (fromRow * 8 + col);
          }
        } else if (fromCol == toCol) { // Same column (vertical)
          for (int row = std::min(fromRow, toRow) + 1;
               row < std::max(fromRow, toRow); ++row) {
            between |= bitboard(1) << (row * 8 + fromCol);
          }
        } else if (std::abs(fromRow - toRow) ==
                   std::abs(fromCol - toCol)) {  // Same diagonal
          int rowStep = (toRow > fromRow) ? 1 : -1;
          int colStep = (toCol > fromCol) ? 1 : -1;
          for (int step = 1; step < abs(toRow - fromRow); ++step) {
            between |= bitboard(1) << ((fromRow + step * rowStep) * 8 +
                                       (fromCol + step * colStep));
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
  constexpr static std::array<bitboard, 128> precalculated_magic{
      9331458702791954561ull,  18014690835701760,
      72075221663744256,       1188959099868286976,
      4971982784777826560,     5476553077400012816,
      900792493384139008,      4935945917447685760,
      140808374272000,         20407210741858432,
      140874935734272,         5188287680018907264,
      1688867580152320,        2342012552319337472,
      4684447308505426176,     18295874595684608,
      18016047781676066,       9009673424355429,
      4715805656752128,        118360777731082240,
      180162676926775300,      216455356669274130,
      2359978563987441665,     4629711412057673860,
      324268253805289476,      2380328327081639936,
      653059406677803136,      4613596978228514880,
      1152925904809232384,     4612251169559938048,
      2314868539390427649,     4406636464257,
      3098429169344640,        292734250661191688,
      1161933239632281858,     5189554283204325377,
      873988600935482368,      141089684065281,
      54184044694339840,       429530284161,
      594477908723073024,      580965726857216000,
      1159819841635483696,     52789445197832,
      11343661598048272,       563023370649636,
      6953566629360042000,     73254139782103045,
      1407651908986112,        2324631468282773760,
      1171503320381014528,     721789820677947520,
      4901042844321972608,     288934080775848320,
      5188287654248120448,     4657025499792589312,
      23925650045801515,       4612005701568495745,
      11540751607664649,       576465873531850753,
      75716803104347669,       8162808751456773,
      1154065551020541444,     554655352898,
      20301419236165664,       2310984330215751696,
      3461018529841087488,     63226868689633316,
      36611606901690368,       576900574671306753,
      9148692793664642,        576742399112905760,
      2882308229692924032,     1188959166723653664,
      5669636038328832,        10450607350715793408ull,
      1130315670097992,        4646546071364048,
      4611686328336648704,     216736834129037442,
      1154047440495052321,     1407546816496644,
      4756931513180754180,     9367773201101291520ull,
      1409608803944977,        563135039799808,
      4611826774306459654,     35185513076737,
      5770241569078642688,     4900778412250105344,
      566258286723680,         577587751723993216,
      166914955545690113,      4504701288876041,
      9262778812776072208ull,  10395293239837197344ull,
      13839579315954132501ull, 2315414343885260816,
      4621010495649351840,     5296797212326756928,
      586594951245332544,      72356749247578176,
      9512202900980498568ull,  9297683630805581908ull,
      2380155738645071920,     565291247478016,
      72571257618688,          9295429906043243010ull,
      4755805673439101952,     1157460290932056322,
      144724334714291240,      2261712600608896,
      4638940782786512234,     9370304178572296192ull,
      14645658226131456,       2887317554974556688,
      432416210307645440,      4707552919616,
      578730487980294144,      326528582396563460,
      37417789247490,          11673897943064643584ull,
      1688858467017216,        9225659570905687041ull,
      1605533271519986688,     1831821005357576,
      1143509944149008,        2310399454299570500};

  constexpr bitboard generateMagic(int &index) {
    if (index < 128)
      return precalculated_magic[index++];
    else
      return 0;
  }

    unsigned processRookIndex(uint8_t pos, bitboard borders) const
    {
        return ((borders & rook_masks[pos]) * rook_magic[pos]) >> rook_shifts[pos];
    }

    unsigned processBishopIndex(uint8_t pos, bitboard borders) const
    {
        return ((borders & bishop_masks[pos]) * bishop_magic[pos]) >> bishop_shifts[pos];
    }

    constexpr MagicConsts()
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

const MagicConsts g_magic_consts{};

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
