#ifndef PARSINGTOOLS_HPP
#define PARSINGTOOLS_HPP

#include <string_view>
#include <vector>

struct TagsParserError {};
struct ImpossibleMoveError {};
struct LexicalParserError {};
struct UnknownParserError {};

class BitBoard;
class Match;

void LoadFromFen(std::string_view fen, BitBoard &board, size_t &index);

std::string SaveToFen(const BitBoard &board);

std::vector<std::string_view> Split(std::string_view data,
                                    const std::vector<char> &seperators);

#endif
