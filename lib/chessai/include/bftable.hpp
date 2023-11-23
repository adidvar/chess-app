#ifndef BFTABLE_HPP
#define BFTABLE_HPP

#include <array>
#include <vector>

#include "turn.hpp"

class BFTable {
  using Frame = std::array<std::array<int, 64>, 64>;
  using Data = std::array<Frame, 64>;

 public:
  BFTable();
  ~BFTable();

  void Push(Turn turn, int depth);
  [[nodiscard]] size_t Get(Turn turn, int depth) const;

  void Clear();

 private:
  Data *m_tables;
};

#endif
