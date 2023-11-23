#ifndef TTABLE_HPP
#define TTABLE_HPP

#include <vector>

#include "bitboard.hpp"
#include "evaluate.hpp"

struct TTableItem {
  bitboard_hash_t hash;

  enum Type { PV, FailLow, FailHigh } type;
  
  Score value;
  Turn pv;

  uint8_t depth;

  bool hasvalue = false;
};

class TTable {
 public:
  TTable();
  void Clear();

  TTableItem *Search(bitboard_hash_t hash, bool &founded);
  const TTableItem *Search(bitboard_hash_t hash, bool &founded) const;

  void Write(TTableItem *element);

  float Fill();

 private:
  std::vector<TTableItem> table_;
};

#endif
