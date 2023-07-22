#include <inttypes.h>

#include "position.hpp"

using bitboard_t = uint64_t;
using bitboard_hash_t = uint64_t;

constexpr bitboard_t operator""_b(bitboard_t num) {
  return (bitboard_t)1 << num;
}

constexpr bitboard_t PositionToBitMask(bitboard_t num) {
  return (bitboard_t)1 << num;
}

constexpr bitboard_t PositionToBitMask(Position position) {
  return (bitboard_t)1 << position.Value();
}

constexpr static bitboard_t kzero = 0;
constexpr static bitboard_t kall = ~kzero;

constexpr static bitboard_t krooking_masks[2][2]{
    {56_b + 58_b + 59_b + 60_b, 60_b + 61_b + 62_b + 63_b},
    {0_b + 2_b + 3_b + 4_b, 4_b + 5_b + 6_b + 7_b}};

constexpr static const bitboard_t row_a =
    (1) + (1 << 8) + (1 << 16) + (1 << 24) + (1LL << 32) + (1LL << 40) +
    (1LL << 48) + (1LL << 56);
constexpr static const bitboard_t row_b = row_a << 1;
constexpr static const bitboard_t row_c = row_a << 2;
constexpr static const bitboard_t row_d = row_a << 3;
constexpr static const bitboard_t row_e = row_a << 4;
constexpr static const bitboard_t row_f = row_a << 5;
constexpr static const bitboard_t row_g = row_a << 6;
constexpr static const bitboard_t row_h = row_a << 7;

constexpr static const bitboard_t rows[8]{row_a, row_b, row_c, row_d,
                                          row_e, row_f, row_g, row_h};

constexpr static const bitboard_t line_8 = (1) + (1 << 1) + (1 << 2) +
                                           (1 << 3) + (1LL << 4) + (1LL << 5) +
                                           (1LL << 6) + (1LL << 7);
constexpr static const bitboard_t line_7 = line_8 << 8;
constexpr static const bitboard_t line_6 = line_8 << 16;
constexpr static const bitboard_t line_5 = line_8 << 24;
constexpr static const bitboard_t line_4 = line_8 << 32;
constexpr static const bitboard_t line_3 = line_8 << 40;
constexpr static const bitboard_t line_2 = line_8 << 48;
constexpr static const bitboard_t line_1 = line_8 << 56;

constexpr static const bitboard_t lines[8]{line_1, line_2, line_3, line_4,
                                           line_5, line_6, line_7, line_8};

struct BitIterator {
  constexpr BitIterator(bitboard_t value) : value_(value), bit_(0) {
    operator++();
  }

  constexpr bitboard_t Value() const { return value_; }
  constexpr bitboard_t Bit() const { return bit_; }
  constexpr void operator=(bitboard_t value) {
    value_ = value;
    operator++();
  }
  constexpr void operator++() {
    bit_ = value_ ^ ((value_ - 1) & value_);
    value_ &= ~bit_;
  }
  constexpr bool Valid() { return bit_ != 0; }

  bitboard_t value_;
  bitboard_t bit_;
};
