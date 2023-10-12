#include "bftable.hpp"

BFTable::BFTable() { m_tables = new Type{}; }

BFTable::~BFTable() { delete m_tables; }

void BFTable::Push(Turn turn, int depth) {
  m_tables->at(depth).at(turn.from().Value()).at(turn.to().Value())++;
}

size_t BFTable::Get(Turn turn, int depth) const {
  return m_tables->at(depth).at(turn.from().Value()).at(turn.to().Value());
}
