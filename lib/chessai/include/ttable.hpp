#ifndef TTABLE_HPP
#define TTABLE_HPP

#include <vector>

#include "bitboard.hpp"
#include "score.hpp"

struct TTableItem {
  bitboard_hash_t hash;

  enum Type { PV, FailLow, FailHigh } type;
  
  Score value;
  Turn pv;

  uint8_t depth;

};

class TTable {
 public:
  TTable();
  void Clear();
  void ClearNoTriggered();

  const TTableItem *Search(bitboard_hash_t hash, bool &founded) const;

  void Write(bitboard_hash_t hash, Score alpha, Score beta, Score value,
             Turn pv, int depth, int depthmax);

 private:
  std::vector<TTableItem> table_;
  std::vector<bool> used_;
  mutable std::vector<bool> triggered_;
};

#endif
