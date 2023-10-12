#ifndef BFTABLE_HPP
#define BFTABLE_HPP

#include <array>
#include <vector>

#include "turn.hpp"

class BFTable {
  using Type = std::array<std::array<std::array<int, 64>, 64>, 64>;

 public:
  BFTable();
  ~BFTable();
  void Push(Turn turn, int depth);
  [[nodiscard]] size_t Get(Turn turn, int depth) const;

 private:
  Type *m_tables;
};

#endif
