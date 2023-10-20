#include <cinttypes>

#include "position.hpp"

using bitboard_t = uint64_t;
using bitboard_hash_t = uint64_t;

constexpr static const bitboard_t kzero = 0;
constexpr static const bitboard_t kall = ~kzero;

constexpr bitboard_t operator""_b(bitboard_t num) {
  return static_cast<bitboard_t>(1) << num;
}

constexpr bitboard_t PositionToBitMask(bitboard_t num) {
  return static_cast<bitboard_t>(1) << num;
}

constexpr bitboard_t PositionToBitMask(Position position) {
  return static_cast<bitboard_t>(1) << position.Value();
}

constexpr static bitboard_t krooking_masks[2][2]{
    {56_b + 58_b + 59_b + 60_b, 60_b + 61_b + 62_b + 63_b},
    {0_b + 2_b + 3_b + 4_b, 4_b + 5_b + 6_b + 7_b}};

constexpr static const bitboard_t row_a =
    (1ULL) + (1ULL << 8ULL) + (1ULL << 16ULL) + (1ULL << 24ULL) +
    (1ULL << 32ULL) + (1ULL << 40ULL) + (1ULL << 48ULL) + (1ULL << 56ULL);
constexpr static const bitboard_t row_b = row_a << 1ULL;
constexpr static const bitboard_t row_c = row_a << 2ULL;
constexpr static const bitboard_t row_d = row_a << 3ULL;
constexpr static const bitboard_t row_e = row_a << 4ULL;
constexpr static const bitboard_t row_f = row_a << 5ULL;
constexpr static const bitboard_t row_g = row_a << 6ULL;
constexpr static const bitboard_t row_h = row_a << 7ULL;

constexpr static const bitboard_t rows[8]{row_a, row_b, row_c, row_d,
                                          row_e, row_f, row_g, row_h};

constexpr static const bitboard_t line_8 =
    (1ULL) + (1ULL << 1ULL) + (1ULL << 2ULL) + (1ULL << 3ULL) + (1ULL << 4ULL) +
    (1ULL << 5ULL) + (1ULL << 6ULL) + (1ULL << 7ULL);

constexpr static const bitboard_t line_7 = line_8 << 8ULL;
constexpr static const bitboard_t line_6 = line_8 << 16ULL;
constexpr static const bitboard_t line_5 = line_8 << 24ULL;
constexpr static const bitboard_t line_4 = line_8 << 32ULL;
constexpr static const bitboard_t line_3 = line_8 << 40ULL;
constexpr static const bitboard_t line_2 = line_8 << 48ULL;
constexpr static const bitboard_t line_1 = line_8 << 56ULL;

constexpr static const bitboard_t lines[8]{line_1, line_2, line_3, line_4,
                                           line_5, line_6, line_7, line_8};

struct BitIterator {
  constexpr BitIterator(bitboard_t value) : m_value(value), m_bit{} {
    operator++();
  }

  [[nodiscard]] constexpr bitboard_t Value() const { return m_value; }
  [[nodiscard]] constexpr bitboard_t Bit() const { return m_bit; }
  constexpr BitIterator& operator=(bitboard_t value) {
    m_value = value;
    operator++();
    return *this;
  }
  constexpr void operator++() {
    m_bit = m_value ^ ((m_value - 1) & m_value);
    m_value &= ~m_bit;
  }
  [[nodiscard]] constexpr bool Valid() const { return m_bit != 0; }

 private:
  bitboard_t m_value;
  bitboard_t m_bit;
};
