#include "hktable.hpp"

#include <cassert>

void HKTable::Increment(Turn turn, uint8_t reversed_depth) {
  assert(turn.Valid());

  if (reversed_depth >= m_killer_table.size()) [[unlikely]] {
    m_killer_table.resize(reversed_depth + 1);
    m_killer_table[reversed_depth]
        .data[turn.from().Value()][turn.to().Value()] += 1;
  }
  m_killer_table[reversed_depth].data[turn.from().Value()][turn.to().Value()] +=
      1;
  // *** TO DO transformation table
  // m_history_table.data[turn.from().Value()][turn.to().Value()] += depth *
  // depth;
}

size_t HKTable::GetKillerMoveCount(Turn turn, uint8_t reversed_depth) const {
  assert(turn.Valid());
  if (reversed_depth >= m_killer_table.size()) [[unlikely]]
    return 0;
  return m_killer_table[reversed_depth]
      .data[turn.from().Value()][turn.to().Value()];
}

size_t HKTable::GetHistoryCount(Turn turn) const {
  assert(turn.Valid());
  return m_history_table.data[turn.from().Value()][turn.to().Value()];
}

void HKTable::Clear() {
  for (auto &elem : m_killer_table)
    for (auto &elem_1 : elem.data)
      for (auto &elem_2 : elem_1) elem_2 = 0;
  for (auto &elem_1 : m_history_table.data)
    for (auto &elem_2 : elem_1) elem_2 = 0;
}
