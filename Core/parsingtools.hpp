#ifndef PARSINGTOOLS_HPP
#define PARSINGTOOLS_HPP

#include <string_view>
#include <turn.hpp>
#include <vector>

class BitBoard;
class Match;

void LoadFromFen(std::string_view fen, BitBoard &board, size_t &index);
std::string SaveToFen(BitBoard board);

std::string_view ReadUntillDelims(std::string_view data,
                                  const std::vector<char> &seperators,
                                  size_t &index);
std::vector<std::string_view> SplitByDelims(
    std::string_view data, const std::vector<char> &seperators);

#endif
