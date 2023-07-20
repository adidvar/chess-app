#ifndef PARSINGTOOLS_HPP
#define PARSINGTOOLS_HPP

#include <vector>
#include <string_view>

#include <turn.hpp>

class BitBoard;
class Match;

void LoadFromFen(std::string_view fen , BitBoard& board);
std::string SaveToFen(BitBoard board);

std::vector<std::string_view> SplitByDelims(std::string_view data, const std::vector<char> &seperators);
std::pair<std::vector<Turn>,BitBoard> ParseTurns(std::string_view data, BitBoard start_pos);
Turn ParseAndExecuteTurn(std::string_view data, BitBoard& board);
std::vector<Match> LoadFromPGN(std::string text);

#endif
