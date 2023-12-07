#include "bftable.hpp"

void BFTable::Increment(Turn turn, int depth, int max_depth) {
  if ((max_depth - depth) < m_tables.size()) {
    m_tables[max_depth - depth].data[turn.from().Value()][turn.to().Value()]++;
  } else {
    m_tables.resize((max_depth - depth) + 1);
    m_tables[max_depth - depth].data[turn.from().Value()][turn.to().Value()]++;
  }
}

size_t BFTable::Get(Turn turn, int depth, int max_depth) const {
  if ((max_depth - depth) < m_tables.size())
    return m_tables[max_depth - depth]
        .data[turn.from().Value()][turn.to().Value()];
  else
    return 0;
}

void BFTable::Clear() { m_tables.clear(); }
