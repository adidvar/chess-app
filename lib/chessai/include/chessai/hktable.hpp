#pragma once

#include <array>
#include <deque>

#include <chesscore/figures.hpp>

class HKTable {
 public:
  void increment(Turn turn, unsigned depth);
  [[nodiscard]] size_t getK(Turn turn, unsigned depth) const;
  [[nodiscard]] size_t getH(Turn turn) const;
  void clear();

 private:
  struct Frame {
    std::array<std::array<size_t, 64>, 64> data{};
  };
  using Data = std::deque<Frame>;

  Data m_killer_table;
  Frame m_history_table;
};

inline void HKTable::increment(Turn turn, unsigned depth) {
  if (depth >= m_killer_table.size()) [[unlikely]] {
    m_killer_table.resize(depth + 1);
    m_killer_table[depth].data[turn.from().index()][turn.to().index()] += 1;
  }
  m_killer_table[depth].data[turn.from().index()][turn.to().index()] += 1;
  ///@todo transformation table
  // m_history_table.data[turn.from().Value()][turn.to().Value()] += depth *
  // depth;
}

inline size_t HKTable::getK(Turn turn, unsigned depth) const {
  if (depth >= m_killer_table.size()) [[unlikely]]
    return 0;
  return m_killer_table[depth].data[turn.from().index()][turn.to().index()];
}

inline size_t HKTable::getH(Turn turn) const {
  return m_history_table.data[turn.from().index()][turn.to().index()];
}

inline void HKTable::clear() {
  for (auto &elem : m_killer_table)
    for (auto &elem_1 : elem.data)
      for (auto &elem_2 : elem_1) elem_2 = 0;
  for (auto &elem_1 : m_history_table.data)
    for (auto &elem_2 : elem_1) elem_2 = 0;
}
