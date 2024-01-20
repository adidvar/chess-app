#ifndef SEARCHSETTINGS_HPP
#define SEARCHSETTINGS_HPP

#include "bitboard.hpp"

class GameStage {
  int stage;

 public:
  [[nodiscard]] bool IsMiddleGame() const { return true; }
  [[nodiscard]] bool IsEndSpiel() const { return false; }
  // value [0-100] describes stage of our period
  [[nodiscard]] int Stage() const { return 50; }
};

class SearchSettings {
 public:
  SearchSettings() {}                          // universal settings
  SearchSettings(const BitBoard& position) {}  // settings for board[position]

  [[nodiscard]] GameStage GetStage() const { return stage; }

  bool NullMoveEnabled() const { return stage.IsMiddleGame(); }
  bool NullMoveR() const { return 2; }

 private:
  GameStage stage;
};

#endif
