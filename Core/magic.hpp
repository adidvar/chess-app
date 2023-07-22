#ifndef MAGIC_HPP
#define MAGIC_HPP

#include <bitboard.hpp>
#include <cinttypes>
#include <figures.hpp>

bitboard_t log2_64(bitboard_t value);
int count_1s(bitboard_t b);

bitboard_t ProcessRook(bitboard_t sq, bitboard_t borders);
bitboard_t ProcessBishop(bitboard_t sq, bitboard_t borders);
bitboard_t AttackFrom(bitboard_t sq);

void InitMagic();

#endif
