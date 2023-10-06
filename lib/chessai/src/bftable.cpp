#include "bftable.hpp"

BFTable::BFTable() {
  table_.resize(64);
  for (auto &elem : table_) elem.resize(64);
}

void BFTable::Push(Turn turn) {
  table_[turn.from().Value()][turn.to().Value()]++;
  counter_++;
}

int BFTable::Get(Turn turn) const {
  return table_[turn.from().Value()][turn.to().Value()];
}
