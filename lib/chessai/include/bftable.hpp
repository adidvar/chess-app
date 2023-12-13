#ifndef BFTABLE_HPP
#define BFTABLE_HPP

#include <array>
#include <deque>
#include <vector>

#include "turn.hpp"

class BFTable {
  struct Frame {
    std::array<std::array<unsigned, 64>, 64> data{};
  };
  using Data = std::deque<Frame>;

 public:
  void Increment(Turn turn, int depth, int max_depth);
  [[nodiscard]] size_t GetKiller(Turn turn, int depth, int max_depth) const;
  [[nodiscard]] size_t GetHistory(Turn turn) const;

  void Clear();

 private:
  Data m_killer_table;
  Frame m_buterfly_table;
};

#endif
