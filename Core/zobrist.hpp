#ifndef ZOBRIST_HPP
#define ZOBRIST_HPP

#include <figures.hpp>
#include <position.hpp>
#include <bitboard.hpp>

bitboard_t GetFigureHash(Figure figure, Color color, Position position);
bitboard_t GetShortCastlingFlagHash(Color color);
bitboard_t GetLongCastlingFlagHash(Color color);
bitboard_t GetTurnCounterHash(int count);
bitboard_t GetPassiveCountHash(int count);
bitboard_t GetCurrentColorHash(Color color);
bitboard_t GetElpassantHash(Position position);

void InitZobrist();

#endif
