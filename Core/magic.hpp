#ifndef MAGIC_HPP
#define MAGIC_HPP

#include <cinttypes>
#include <figures.hpp>

constexpr static const bitboard_t row_a = (1) + (1<<8) + (1<<16) + (1<<24) + (1LL<<32) + (1LL<<40) + (1LL<<48) + (1LL<<56);
constexpr static const bitboard_t row_b = row_a << 1;
constexpr static const bitboard_t row_c = row_a << 2;
constexpr static const bitboard_t row_d = row_a << 3;
constexpr static const bitboard_t row_e = row_a << 4;
constexpr static const bitboard_t row_f = row_a << 5;
constexpr static const bitboard_t row_g = row_a << 6;
constexpr static const bitboard_t row_h = row_a << 7;

constexpr static const bitboard_t rows[8]{
    row_a,row_b,row_c,row_d,row_e,row_f,row_g,row_h
};

constexpr static const bitboard_t line_8 = (1) + (1<<1) + (1<<2) + (1<<3) + (1LL<<4) + (1LL<<5) + (1LL<<6) + (1LL<<7);
constexpr static const bitboard_t line_7 = line_8 << 8;
constexpr static const bitboard_t line_6 = line_8 << 16;
constexpr static const bitboard_t line_5 = line_8 << 24;
constexpr static const bitboard_t line_4 = line_8 << 32;
constexpr static const bitboard_t line_3 = line_8 << 40;
constexpr static const bitboard_t line_2 = line_8 << 48;
constexpr static const bitboard_t line_1 = line_8 << 56;

constexpr static const bitboard_t lines[8]{
    line_1 , line_2 , line_3 , line_4 , line_5 , line_6 , line_7 , line_8
};

bitboard_t log2_64(bitboard_t value);
int count_1s(bitboard_t b);

bitboard_t ProcessRook(bitboard_t sq, bitboard_t borders);
bitboard_t ProcessBishop(bitboard_t sq, bitboard_t borders);
bitboard_t AttackFrom(bitboard_t sq);

void InitMagic();

#endif
