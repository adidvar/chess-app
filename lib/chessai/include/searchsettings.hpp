#ifndef SEARCHSETTINGS_HPP
#define SEARCHSETTINGS_HPP

#include "bitboard.hpp"

class GameStage {
  int stage;
  // [0 - 100) opening
  // [100 - 200) middlegame
  // [200 - 300) endspiel

 public:
  [[nodiscard]] bool IsOpening() const { return false; }
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

 private:
  GameStage stage;
};

#endif
