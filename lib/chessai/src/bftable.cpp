#include "bftable.hpp"

BFTable::BFTable() { m_tables = new Data{}; }

BFTable::~BFTable() { delete m_tables; }

void BFTable::Push(Turn turn, int depth) {
  m_tables->at(depth).at(turn.from().Value()).at(turn.to().Value())++;
}

size_t BFTable::Get(Turn turn, int depth) const {
  return m_tables->at(depth).at(turn.from().Value()).at(turn.to().Value());
}

void BFTable::Clear() {
  for (auto &frame : *m_tables)
    for (auto &vect : frame)
      for (auto &value : vect) value = 0;
}
