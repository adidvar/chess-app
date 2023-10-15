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
  if (founded) element->used = true;
  return element;
}

const SearchElement *TTable::Search(bitboard_hash_t hash, bool &founded) const {
  const SearchElement *element = &table_[hash % table_.size()];
  founded = element->hasvalue && element->hash == hash;
  return element;
}

float TTable::Fill() {
  int sum = 0;
  for (auto &elem : table_) sum += elem.hasvalue;
  return sum * 100.0 / table_.size();
}

float TTable::Used() {
  int sum = 0;
  for (auto &elem : table_) sum += elem.used;
  return sum * 100.0 / table_.size();
}

void TTable::ClearUsedFlag() {
  for (auto &elem : table_) {
    // if (elem.used == false) elem.hasvalue = false;
    elem.used = false;
  }
}
