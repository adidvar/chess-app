#pragma once

#include <chesscore/figures.hpp>
#include <thread>

#include "search.hpp"

class Computer{
 public:
  Computer();
  ~Computer();

  void SetBoard(const BitBoard &board);

  void Start();
  void Abort();
 private:
  BitBoard m_board;
};
