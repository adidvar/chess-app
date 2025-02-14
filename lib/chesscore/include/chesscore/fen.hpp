#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

class BitBoard;

class FenError : public std::runtime_error {
 public:
  explicit FenError(const std::string &message)
      : std::runtime_error("FEN Parsing Error: " + message) {}

  explicit FenError(const char *message)
      : std::runtime_error(std::string("FEN Parsing Error: ") + message) {}
};

void boardFromFen(std::string_view fen, BitBoard &board, size_t &index);
std::string boardToFen(const BitBoard &board);
