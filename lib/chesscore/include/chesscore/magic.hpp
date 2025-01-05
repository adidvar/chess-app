#ifndef MAGIC_HPP
#define MAGIC_HPP

#include <cinttypes>

#include "bitboard.hpp"

BitBoard::bitboard processRook(Position pos, BitBoard::bitboard borders);
/**
 * @brief processBishop returns a bishop attack bitboard
 * @param sq input figure
 * @param borders borders
 * @return processed attack bitboard
 */
BitBoard::bitboard processBishop(Position pos, BitBoard::bitboard borders);
/**
 * @brief attackFrom generates common attack bitboard for knight bitshop and
 * rook together
 * @param sq input position
 * @return processed bitboard
 */
BitBoard::bitboard attackFrom(BitBoard::bitboard sq);
/**
 * @brief initMagic inits magic numbers for magic bitboard
 * @warning Don`t use functions in this file before this call
 */
void initMagic();

#endif
