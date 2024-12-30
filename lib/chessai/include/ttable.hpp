#ifndef TTABLE_HPP
#define TTABLE_HPP

#include <vector>

#include <chesscore/bitboard.hpp>
#include "score.hpp"
/**
 * @brief The TTableItem class element of transposition table
 */
struct TTableItem {
  bitboard_hash_t hash{};  ///< hash of item

  enum Type { PV, FailLow, FailHigh } type;  ///< node type

  Score value;  ///< value of node
  Turn pv;      ///< pv of node

  uint8_t depth{};  ///< depth of last scan
};

/**
 * @brief The TTable class is a transposition table
 */
class TTable {
 public:
  /**
   * @brief TTable default constructor
   */
  TTable();
  /**
   * @brief Clear clears a table
   */
  void Clear();
  /**
   * @brief ClearGarbage clears garbage elements of ttable
   */
  void ClearGarbage();
  /**
   * @brief ClearGarbageFlag sets all elements garbage flag to true
   */
  void SetGarbageFlag();

  /**
   * @brief Search search element from table
   * @param hash hash for search
   * @param founded [out] is the element founded
   * @return pointer to ttableitem node
   */
  const TTableItem *Search(bitboard_hash_t hash, bool &founded) const;

  /**
   * @brief Write stores node info into ttable
   * @param hash hash of node
   * @param alpha alpha
   * @param beta beta
   * @param value value
   * @param pv pv
   * @param depth depth
   * @param depthmax depthmax
   */
  void Write(bitboard_hash_t hash, Score alpha, Score beta, Score value,
             Turn pv, uint8_t depth);

 private:
  size_t m_size;
  std::vector<TTableItem> m_table;
  std::vector<bool> m_used;
  mutable std::vector<bool> m_garbage;
};

#endif
