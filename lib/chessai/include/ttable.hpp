#ifndef TTABLE_HPP
#define TTABLE_HPP

#include <vector>

#include "bitboard.hpp"
#include "score.hpp"

struct TTableItem {
  bitboard_hash_t hash;

  enum Type { PV, FailLow, FailHigh } type;
  
  Score value;
  Turn pv;

  uint8_t depth;

};

class TTable {
 public:
  TTable();
  void Clear();
  void ClearNoTriggered();

  const TTableItem *Search(bitboard_hash_t hash, bool &founded) const;

  void Write(bitboard_hash_t hash, Score alpha, Score beta, Score value,
             Turn pv, int depth, int depthmax);

  TTableItem GetLastElement() const;
  void SetLastElement(const TTableItem &);

 private:
  std::vector<TTableItem> m_table;
  std::vector<bool> m_used;
  mutable std::vector<bool> m_triggered;

  TTableItem m_last_elem;
};

#endif
