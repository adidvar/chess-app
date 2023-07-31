#include "searchtree.hpp"

void GameTreeLayer::Init(const TreeEntry &parent) {
  const BitBoard &board = parent.board;
}

void GameTreeLayer::Next() {}

bool GameTreeLayer::IsEnd() const { return entries_.size() == current_index_; }

GameTree::GameTree(const BitBoard &board) {}

const TreeEntry &GameTree::GetCurrentEntry() {
  return layers_[depth_].GetCurrentEntry();
}

bool GameTree::IsMoves() { return layers_[depth_].IsEnd(); }

void GameTree::DoMove() {}

void GameTree::UndoMove() {}
