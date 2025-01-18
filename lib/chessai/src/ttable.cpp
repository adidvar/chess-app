#include "ttable.hpp"

constexpr size_t mb = 4;

TTable::TTable() {
  size_t bytes = mb * 1024 * 1024;
  m_size = bytes / sizeof(TTableItem);

  m_used.resize(m_size, false);
  m_table.resize(m_size);
  m_garbage.resize(m_size, true);
}

void TTable::clear() {
  for (auto &&i : m_used) i = false;
  for (auto &&i : m_garbage) i = true;
}

void TTable::clearGarbage() {
  for (int i = 0; i < m_garbage.size(); i++)
    if (m_garbage[i]) m_used[i] = false;
}

void TTable::setGarbageFlag() {
  for (auto &&i : m_garbage) i = true;
}

const TTableItem *TTable::search(BitBoardHash hash, bool &founded) const
{
    auto index = hash % m_table.size();
    const TTableItem *element = &m_table[index];
    founded = (m_used[index] && element->hash == hash);
    m_garbage[index] = m_garbage[index] & !founded;
    return element;
}

void TTable::write(BitBoardHash hash, Score alpha, Score beta, Score value, Turn pv, uint8_t depth)
{
    auto index = hash % m_table.size();
    auto used = m_used[index];
    auto triggered = m_garbage[index];
    auto &element = m_table[index];

    if (used && element.depth > depth)
        return;

    if (value <= alpha)
        element.type = TTableItem::FailLow;
    else if (value >= beta)
        element.type = TTableItem::FailHigh;
    else
        element.type = TTableItem::PV;

    element.value = value;
    element.depth = depth;
    element.hash = hash;
    element.pv = pv;

    m_garbage[index] = false;
    m_used[index] = true;
}
