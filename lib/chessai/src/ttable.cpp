#include "ttable.hpp"

constexpr size_t mb = 4;

TTable::TTable() {
  size_t bytes = mb * 1024 * 1024;
  size_t size = bytes / sizeof(TTableItem);
  m_table.resize(size);
  m_used.resize(size, false);
  m_triggered.resize(size, false);
}

void TTable::Clear() {
  for (int i = 0; i < m_used.size(); i++) m_used[i] = false;
  for (int i = 0; i < m_used.size(); i++) m_triggered[i] = false;
}

void TTable::ClearNoTriggered() {
  for (int i = 0; i < m_triggered.size(); i++) {
    if (!m_triggered[i]) m_used[i] = false;
    m_triggered[i] = false;
  }
}

const TTableItem *TTable::Search(bitboard_hash_t hash, bool &founded) const {
  auto index = hash % m_table.size();
  const TTableItem *element = &m_table[index];
  founded = m_used[index] && element->hash == hash;
  m_triggered[index] = m_triggered[index] | founded;
  return element;
}

void TTable::Write(bitboard_hash_t hash, Score alpha, Score beta, Score value,
                   Turn pv, uint8_t depth) {
  auto index = hash % m_table.size();
  auto used = m_used[index];
  auto triggered = m_triggered[index];
  auto &element = m_table[index];

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

  m_used[index] = true;
  m_triggered[index] = true;
}
