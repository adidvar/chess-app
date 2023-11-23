#ifndef BOARDITERATOR_HPP
#define BOARDITERATOR_HPP

#include <vector>

#include "bitboard.hpp"

class ChessTree {
 public:
  std::vector<BitBoardTuple>& Get(size_t depth);

 private:
};

#endif
