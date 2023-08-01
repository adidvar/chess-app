#include "ttable.hpp"

constexpr size_t mb = 64;

TTable::TTable() {
  size_t bytes = mb * 1024 * 1024;
  size_t size = bytes / sizeof(SearchElement);
  table_.resize(size);
}

void TTable::Clear() {
  for (auto &elem : table_) elem.hasvalue = false;
}

SearchElement *TTable::Search(bitboard_hash_t hash, bool &founded) {
  SearchElement *element = &table_[hash % table_.size()];
  founded = element->hasvalue && element->hash == hash;
  return element;
}
