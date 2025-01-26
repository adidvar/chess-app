#pragma once

#include <vector>

#include <chesscore/bitboard.hpp>
#include "score.hpp"

struct TTableItem {
    BitBoardHash hash{}; ///< hash of item
    Score value;         ///< value of node
    Turn pv;     ///< pv of node
    uint8_t depth{}; ///< depth of last scan
    enum struct Type : uint8_t { PV, FailLow, FailHigh } type;  ///< node type
};

class TTable {
 public:
  TTable();

  void clear();
  void clearGarbage();
  void setGarbageFlag();

  const TTableItem *search(BitBoardHash hash, bool &founded) const;
  void write(BitBoardHash hash, Score alpha, Score beta, Score value, Turn pv,
             uint8_t depth);

 private:
  size_t m_size;
  std::vector<TTableItem> m_table;
  std::vector<bool> m_used;
  mutable std::vector<bool> m_garbage;
};
