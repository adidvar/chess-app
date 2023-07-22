#ifndef MAGIC_HPP
#define MAGIC_HPP

#include <cinttypes>

#include "bitboard.hpp"

/**
 * @brief log2_64 returns fast log of 64 bit number
 * @param value input value
 * @return
 */
bitboard_t log2_64(bitboard_t value);
/**
 * @brief count_1s returns count of 1 bits in bitboard
 * @param b
 * @return
 */
int count_1s(bitboard_t b);
/**
 * @brief ProcessRook returns a rook attack bitboard
 * @param sq input figure
 * @param borders borders
 * @return processed attack bitboard
 */
bitboard_t ProcessRook(bitboard_t sq, bitboard_t borders);
/**
 * @brief ProcessBishop returns a bishop attack bitboard
 * @param sq input figure
 * @param borders borders
 * @return processed attack bitboard
 */
bitboard_t ProcessBishop(bitboard_t sq, bitboard_t borders);
/**
 * @brief AttackFrom generates common attack bitboard for knight bitshop and
 * rook together
 * @param sq input position
 * @return processed bitboard
 */
bitboard_t AttackFrom(bitboard_t sq);
/**
 * @brief InitMagic inits magic numbers for magic bitboard
 * @warning Don`t use functions in this file before this call
 */
void InitMagic();

#endif
