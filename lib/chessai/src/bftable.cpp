#include "bftable.hpp"

void BFTable::Increment(Turn turn, int depth, int max_depth) {
  if ((max_depth - depth) < m_killer_table.size()) {
    m_killer_table[max_depth - depth]
        .data[turn.from().Value()][turn.to().Value()] += 1;
  } else {
    m_killer_table.resize((max_depth - depth) + 1);
    m_killer_table[max_depth - depth]
        .data[turn.from().Value()][turn.to().Value()] += 1;
  }
  m_buterfly_table.data[turn.from().Value()][turn.to().Value()] +=
      depth * depth;
}

size_t BFTable::GetKiller(Turn turn, int depth, int max_depth) const {
  if ((max_depth - depth) < m_killer_table.size())
    return m_killer_table[max_depth - depth]
        .data[turn.from().Value()][turn.to().Value()];
  else
    return 0;
}

size_t BFTable::GetHistory(Turn turn) const {
  return m_buterfly_table.data[turn.from().Value()][turn.to().Value()];
}

void BFTable::Clear() {}
