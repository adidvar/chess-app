#include "magic.hpp"

#include <cassert>
#include <random>
#include <stdexcept>

// all magic in 16 lines
// masks index - position
static bitboard_t RMask1[64];
static bitboard_t RMask2[64];
static bitboard_t BMask1[64];
static bitboard_t BMask2[64];
// shifts index - position
static bitboard_t RShift1[64];
static bitboard_t RShift2[64];
static bitboard_t BShift1[64];
static bitboard_t BShift2[64];
// magics index - magic
static bitboard_t RMagic1[64];
static bitboard_t RMagic2[64];
static bitboard_t BMagic1[64];
static bitboard_t BMagic2[64];
// hash maps index - position , hash
static bitboard_t RHash1[64][64];
static bitboard_t RHash2[64][64];
static bitboard_t BHash1[64][64];
static bitboard_t BHash2[64][64];
// 22kb total
static bitboard_t AttackFromMask[64];

const int tab64[64] = {63, 0,  58, 1,  59, 47, 53, 2,  60, 39, 48, 27, 54,
                       33, 42, 3,  61, 51, 37, 40, 49, 18, 28, 20, 55, 30,
                       34, 11, 43, 14, 22, 4,  62, 57, 46, 52, 38, 26, 32,
                       41, 50, 36, 17, 19, 29, 10, 13, 21, 56, 45, 25, 31,
                       35, 16, 9,  12, 44, 24, 15, 8,  23, 7,  6,  5};

int count_1s(bitboard_t b) {
  int r;
  for (r = 0; b; r++, b &= b - 1)
    ;
  return r;
}

bitboard_t log2_64(bitboard_t value) {
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  value |= value >> 32;
  return tab64[((bitboard_t)((value - (value >> 1)) * 0x07EDD5E59A4E28C2)) >>
               58];
}

template <typename T>
static bitboard_t random_uint64_fewbits(T& random) {
  return random() & random() & random();
}

static bitboard_t generate_rmask1(bitboard_t sq) {
  bitboard_t result = 0;
  int rk = sq / 8, fl = sq % 8, f;
  for (f = fl + 1; f <= 6; f++) result |= (1ULL << (f + rk * 8));
  for (f = fl - 1; f >= 1; f--) result |= (1ULL << (f + rk * 8));
  return result;
}

static bitboard_t generate_rmask2(bitboard_t sq) {
  bitboard_t result = 0ULL;
  int rk = sq / 8, fl = sq % 8, r;
  for (r = rk + 1; r <= 6; r++) result |= (1ULL << (fl + r * 8));
  for (r = rk - 1; r >= 1; r--) result |= (1ULL << (fl + r * 8));
  return result;
}

static bitboard_t generate_bmask1(bitboard_t sq) {
  bitboard_t result = 0ULL;
  int rk = sq / 8, fl = sq % 8, r, f;
  for (r = rk + 1, f = fl + 1; r <= 6 && f <= 6; r++, f++)
    result |= (1ULL << (f + r * 8));
  for (r = rk - 1, f = fl - 1; r >= 1 && f >= 1; r--, f--)
    result |= (1ULL << (f + r * 8));
  return result;
}

static bitboard_t generate_bmask2(bitboard_t sq) {
  bitboard_t result = 0ULL;
  int rk = sq / 8, fl = sq % 8, r, f;
  for (r = rk - 1, f = fl + 1; r >= 1 && f <= 6; r--, f++)
    result |= (1ULL << (f + r * 8));
  for (r = rk + 1, f = fl - 1; r <= 6 && f >= 1; r++, f--)
    result |= (1ULL << (f + r * 8));
  return result;
}

static bitboard_t generate_rattack1(bitboard_t sq, bitboard_t borders) {
  bitboard_t delta = 1;
  bitboard_t rmask = 0xFFULL << 8 * (sq / 8);
  borders = ~rmask | ~borders;

  bitboard_t result = 0;

  bitboard_t p = 1ULL << sq;
  while (p) {
    p <<= delta;
    result |= p;
    p &= borders;
  }
  p = 1ULL << sq;
  while (p) {
    p >>= delta;
    result |= p;
    p &= borders;
  }
  return result & rmask;
}

static bitboard_t generate_rattack2(bitboard_t sq, bitboard_t borders) {
  bitboard_t delta = 8;
  bitboard_t rmask = 0x101010101010101ULL << (sq % 8);
  borders = ~rmask | ~borders;

  bitboard_t result = 0;

  bitboard_t p = 1ULL << sq;
  while (p) {
    p <<= delta;
    result |= p;
    p &= borders;
  }
  p = 1ULL << sq;
  while (p) {
    p >>= delta;
    result |= p;
    p &= borders;
  }
  return result & rmask;
}

static bitboard_t generate_battack1(bitboard_t sq, bitboard_t borders) {
  bitboard_t result = 0ULL;
  int rk = sq / 8, fl = sq % 8, r, f;
  for (r = rk + 1, f = fl + 1; r <= 7 && f <= 7; r++, f++) {
    result |= (1ULL << (f + r * 8));
    if (borders & (1ULL << (f + r * 8))) break;
  }
  for (r = rk - 1, f = fl - 1; r >= 0 && f >= 0; r--, f--) {
    result |= (1ULL << (f + r * 8));
    if (borders & (1ULL << (f + r * 8))) break;
  }
  return result;
}

static bitboard_t generate_battack2(bitboard_t sq, bitboard_t borders) {
  bitboard_t result = 0ULL;
  int rk = sq / 8, fl = sq % 8, r, f;
  for (r = rk - 1, f = fl + 1; r >= 0 && f <= 7; r--, f++) {
    result |= (1ULL << (f + r * 8));
    if (borders & (1ULL << (f + r * 8))) break;
  }
  for (r = rk + 1, f = fl - 1; r <= 7 && f >= 0; r++, f--) {
    result |= (1ULL << (f + r * 8));
    if (borders & (1ULL << (f + r * 8))) break;
  }
  return result;
}

// movement from value to int64
static bitboard_t value_to_int64(bitboard_t mask, bitboard_t data) {
  bitboard_t counter = 0;
  bitboard_t result = 0;
  while (mask != 0) {
    bitboard_t bit = mask ^ ((mask - 1) & mask);
    bitboard_t value = (data >> counter) & 1uLL;

    if (value) value = 0xFFFFFFFFFFFFFFFF;

    result |= value & bit;

    mask &= mask - 1;
    counter++;
  }
  return result;
}

// search a magic and fill hash table
template <typename T>
static bitboard_t find_magic(bitboard_t sq, bitboard_t mask,
                             bitboard_t (*attack)(bitboard_t, bitboard_t),
                             bitboard_t hash_ref[64][64], T& random) {
  using namespace std;
  size_t n = count_1s(mask);
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
    for (size_t i = 0; i < c; i++) hash[i] = 0;

    if (count_1s(mask * magic >> (64ULL - n)) < n) continue;

    for (size_t i = 0; i < c; i++) {
      uint64_t index = input[i] * magic >> (64 - n);
      // add to hash
      if (hash[index] == 0) hash[index] = output[i];
      // hash miss
      else if (hash[index] != output[i]) {
        flag = false;
        break;
      }
    }
    if (flag) {
      for (size_t i = 0; i < c; i++) hash_ref[sq][i] = hash[i];

      return magic;
    }
  }
  throw std::runtime_error("can't find magic");
}

bitboard_t find_attacks_from(bitboard_t sq) {
  sq = 1ULL << sq;
  bitboard_t mask = 0;
  mask |= ProcessBishop(sq, 0) | ProcessRook(sq, 0);
  mask |= (((sq << 10) & ~(row_a | row_b)) | ((sq << 17) & (~row_a)) |
           ((sq >> 6)) & ~(row_a | row_b) | ((sq >> 15) & ~(row_a)) |
           ((sq << 6) & ~(row_g | row_h)) | ((sq << 15) & ~(row_h)) |
           ((sq >> 10) & ~(row_g | row_h)) | ((sq >> 17) & ~(row_h)));
  ;

  return mask;
}

bitboard_t ProcessRook(bitboard_t sq, bitboard_t borders) {
  sq = log2_64(sq);
  auto a1 =
      RHash1[sq][(borders & RMask1[sq]) * RMagic1[sq] >> (64 - RShift1[sq])];
  auto a2 =
      RHash2[sq][(borders & RMask2[sq]) * RMagic2[sq] >> (64 - RShift2[sq])];
  return a1 | a2;
}

bitboard_t ProcessBishop(bitboard_t sq, bitboard_t borders) {
  sq = log2_64(sq);
  auto a1 =
      BHash1[sq][(borders & BMask1[sq]) * BMagic1[sq] >> (64 - BShift1[sq])];
  auto a2 =
      BHash2[sq][(borders & BMask2[sq]) * BMagic2[sq] >> (64 - BShift2[sq])];
  return a1 | a2;
}

bitboard_t AttackFrom(bitboard_t sq) {
  sq = log2_64(sq);
  return AttackFromMask[sq];
}

void InitMagic() {
  std::mt19937_64 random(0ULL);

  for (size_t i = 0; i < 64; i++) {
    // masks
    RMask1[i] = generate_rmask1(i);
    RMask2[i] = generate_rmask2(i);
    BMask1[i] = generate_bmask1(i);
    BMask2[i] = generate_bmask2(i);
    // shifts
    RShift1[i] = count_1s(RMask1[i]);
    RShift2[i] = count_1s(RMask2[i]);
    BShift1[i] = count_1s(BMask1[i]);
    BShift2[i] = count_1s(BMask2[i]);
    // magics and hashes
    RMagic1[i] = find_magic(i, RMask1[i], generate_rattack1, RHash1, random);
    RMagic2[i] = find_magic(i, RMask2[i], generate_rattack2, RHash2, random);
    BMagic1[i] = find_magic(i, BMask1[i], generate_battack1, BHash1, random);
    BMagic2[i] = find_magic(i, BMask2[i], generate_battack2, BHash2, random);
  }

  for (size_t i = 0; i < 64; i++) AttackFromMask[i] = find_attacks_from(i);
}

struct StartUp {
  StartUp() { InitMagic(); }
} static startup;
