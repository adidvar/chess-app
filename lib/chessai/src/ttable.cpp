#include "ttable.hpp"

constexpr size_t mb = 64;

TTable::TTable() {
  size_t bytes = mb * 1024 * 1024;
  size_t size = bytes / sizeof(TTableItem);
  table_.resize(size);
}

void TTable::Clear() {
  for (auto &elem : table_) elem.hasvalue = false;
}

TTableItem *TTable::Search(bitboard_hash_t hash, bool &founded) {
  TTableItem *element = &table_[hash % table_.size()];
  founded = element->hasvalue && element->hash == hash;
  return element;
}

const TTableItem *TTable::Search(bitboard_hash_t hash, bool &founded) const {
  const TTableItem *element = &table_[hash % table_.size()];
  founded = element->hasvalue && element->hash == hash;
  return element;
}

float TTable::Fill() {
  int sum = 0;
  for (auto &elem : table_) sum += elem.hasvalue;
  return sum * 100.0 / table_.size();
}
