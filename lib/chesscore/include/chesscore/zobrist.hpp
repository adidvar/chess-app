#pragma once

#include "bitboard.hpp"
#include "figures.hpp"
#include "position.hpp"

BitBoard::BitBoardHash getFigureHash(Figure figure, Position position);
BitBoard::BitBoardHash getFlagsHash(BitBoard::Flags flags);

void initZobrist(uint64_t seed);
