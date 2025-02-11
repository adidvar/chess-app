#pragma once

#include <memory>
#include <optional>

#include "chesscore/bitboard.hpp"
#include "ifeedback.hpp"

struct SearchSettings {
  // search parameters
  BitBoard board;
  std::shared_ptr<IFeedBack> feedback{new IFeedBack()};

  // time controll parameters
  const int default_time = 6000;  // when no parameters
  bool infinite = false;
  std::optional<int> depth = 20;
  std::optional<int> mate = 20;
  std::optional<int> nodes = 0;
  std::optional<float> move_time;
  std::optional<float> w_time;
  std::optional<float> w_inc;
  std::optional<float> b_time;
  std::optional<float> b_inc;
  std::optional<float> moves_to_go;

  // additional options
  bool ponder = false;
  std::vector<Turn> search_moves = {};
};
