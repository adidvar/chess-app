#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

class BitBoard;

class FenParsingError : public std::runtime_error
{
public:
    explicit FenParsingError(const std::string &message)
        : std::runtime_error("FEN Parsing Error: " + message)
    {}

    explicit FenParsingError(const char *message)
        : std::runtime_error(std::string("FEN Parsing Error: ") + message)
    {}
};

void boardFromFen(std::string_view fen, BitBoard &board, size_t &index);
std::string boardToFen(const BitBoard &board);
