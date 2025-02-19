#pragma once

#include <limits>
#include <memory>

#include "chesscore/bitboard.hpp"
#include "ifeedback.hpp"

struct SearchSettings {
  // search parameters
  BitBoard board;
  std::shared_ptr<IFeedBack> feedback{new IFeedBack()};

  // stop parameters
  float time =
      std::numeric_limits<float>::max();  // time we have to calculate moves
  long nodes =
      std::numeric_limits<long>::max();  // nodes we have to calculate moves
  int depth =
      std::numeric_limits<int>::max();  // depth we have to calculate moves

  void resetLimits() {
    time = std::numeric_limits<float>::max();
    nodes = std::numeric_limits<long>::max();
    depth = std::numeric_limits<int>::max();
  }

  // additional options
  std::vector<Turn> moves = {};  // moves to analyze
};
