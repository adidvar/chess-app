#ifndef TTABLE_HPP
#define TTABLE_HPP

#include <vector>

#include "bitboard.hpp"
#include "evaluate.hpp"

struct SearchElement {
  bitboard_hash_t hash;

  Evaluate value;

  enum Type { PV, FailLow, FailHigh } type;

  int depth;

  bool hasvalue = false;
};

class TTable {
 public:
  TTable();
  void Clear();
  SearchElement *Search(bitboard_hash_t hash, bool &founded);
  void Write(SearchElement *element);

 private:
  std::vector<SearchElement> table_;
};

#endif
