#ifndef SEARCHTREE_HPP
#define SEARCHTREE_HPP

#include "bitboard.hpp"

/**
 * @brief The TreeEntry class describes a entry in a chess search tree
 */
struct TreeEntry {
  BitBoard board;
  bitboard_hash_t hash;
  Turn turn;
  int priority;
};

class GameTreeLayer {
 public:
  void Init(const TreeEntry& parent);
  const TreeEntry& GetCurrentEntry() const;
  void Next();
  bool IsEnd() const;

 private:
  std::vector<TreeEntry> entries_;
  size_t current_index_ = 0;
};

class GameTree {
 public:
  GameTree(const BitBoard& board);

  const TreeEntry& GetCurrentEntry();
  bool IsMoves();
  void DoMove();
  void UndoMove();

 private:
  int depth_;

  std::vector<GameTreeLayer> layers_;
};

#endif
