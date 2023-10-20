#ifndef TTABLE_HPP
#define TTABLE_HPP

#include <vector>

#include "bitboard.hpp"
#include "evaluate.hpp"

struct SearchElement {
  bitboard_hash_t hash;

  enum Type { PV, FailLow, FailHigh } type;

  Evaluate value;
  Turn pv;

  uint8_t depth;

  bool hasvalue = false;
  bool used = false;
};

class TTable {
 public:
  TTable();
  void Clear();
  SearchElement *Search(bitboard_hash_t hash, bool &founded);
  const SearchElement *Search(bitboard_hash_t hash, bool &founded) const;
  void Write(SearchElement *element);

  float Fill();
  float Used();

  void ClearUsedFlag();

 private:
  std::vector<SearchElement> table_;
};

#endif
