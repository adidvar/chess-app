#ifndef PARSINGTOOLS_HPP
#define PARSINGTOOLS_HPP

#include <string_view>
#include <vector>

class BitBoard;
class Match;

void LoadFromFen(std::string_view fen, BitBoard &board, size_t &index);

std::string SaveToFen(const BitBoard &board);

std::vector<std::string_view> Split(std::string_view data,
                                    const std::vector<char> &seperators);

std::string_view ReadUntillDelims(std::string_view data,
                                  const std::vector<char> &seperators,
                                  size_t &index);

#endif
