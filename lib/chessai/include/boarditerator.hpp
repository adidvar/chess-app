#ifndef BOARDITERATOR_HPP
#define BOARDITERATOR_HPP

#include <vector>

#include "bitboard.hpp"
#include "bitboardtuple.hpp"

class ChessTreeHash {
 public:
  std::vector<BitBoardTuple> &Get(int depth) {
    m_turns.resize(depth + 1);
    return m_turns[depth];
  }

 private:
  std::vector<std::vector<BitBoardTuple>> m_turns;
};

#endif
