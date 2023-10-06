#ifndef BFTABLE_HPP
#define BFTABLE_HPP

#include <vector>

#include "turn.hpp"

class BFTable {
 public:
  BFTable();
  void Push(Turn turn);
  size_t Get(Turn turn) const;

 private:
  std::vector<std::vector<size_t>> table_;
};

#endif
