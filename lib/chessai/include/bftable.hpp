#ifndef BFTABLE_HPP
#define BFTABLE_HPP

#include <vector>

#include "turn.hpp"

class BFTable {
 public:
  BFTable();
  void Push(Turn turn);
  int Get(Turn turn) const;  // 0 - 1000

 private:
  std::vector<std::vector<size_t>> table_;
  size_t counter_;
};

#endif
