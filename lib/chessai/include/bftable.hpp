#ifndef BFTABLE_HPP
#define BFTABLE_HPP

#include <array>
#include <deque>
#include <vector>

#include "turn.hpp"

class BFTable {
  struct Frame {
    unsigned data[64][64];
  };
  using Data = std::deque<Frame>;

 public:
  void Increment(Turn turn, int depth, int max_depth);
  [[nodiscard]] size_t Get(Turn turn, int depth, int max_depth) const;

  void Clear();

 private:
  Data m_tables;
};

#endif
