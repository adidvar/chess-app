#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <turn.hpp>
#include <bitboard.hpp>
#include <position_rating.hpp>

MateAppraiser Evaluate(BitBoard board,Color color);

#endif
