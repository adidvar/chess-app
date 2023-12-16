#include "ttable.hpp"

constexpr size_t mb = 64;

TTable::TTable() {
  size_t bytes = mb * 1024 * 1024;
  size_t size = bytes / sizeof(TTableItem);
  table_.resize(size);
  used_.resize(size, false);
  triggered_.resize(size, false);
}

void TTable::Clear() {
  for (int i = 0; i < used_.size(); i++) used_[i] = false;
}

void TTable::ClearNoTriggered() {
  for (int i = 0; i < triggered_.size(); i++) {
    if (!triggered_[i]) used_[i] = false;
    triggered_[i] = false;
  }
}

const TTableItem *TTable::Search(bitboard_hash_t hash, bool &founded) const {
  auto index = hash % table_.size();
  const TTableItem *element = &table_[index];
  founded = used_[index] && element->hash == hash;
  triggered_[index] = triggered_[index] | founded;
  return element;
}

void TTable::Write(bitboard_hash_t hash, Score alpha, Score beta, Score value,
                   Turn pv, int depth, int depthmax) {
  auto index = hash % table_.size();
  auto used = used_[index];
  auto triggered = triggered_[index];
  auto &element = table_[index];

  if (depth != depthmax)
    if (used && element.depth > depth) return;

  if (value <= alpha)
    element.type = TTableItem::FailLow;
  else if (value >= beta)
    element.type = TTableItem::FailHigh;
  else
    element.type = TTableItem::PV;

  element.hash = hash;
  element.value = value;
  element.pv = pv;
  element.depth = depth;

  used_[index] = true;
  triggered_[index] = true;
}
