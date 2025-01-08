#ifndef MAGIC_HPP
#define MAGIC_HPP

#include <cinttypes>

#include "bitboard.hpp"

bitboard processRook(Position pos, bitboard borders);

bitboard processBishop(Position pos, bitboard borders);

void initMagic();

#endif
