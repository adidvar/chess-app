#pragma once

#include <chessai/alphabeta.hpp>
#include <chesscore/figures.hpp>
#include <thread>

class Computer{
 public:
  Computer();
  ~Computer();

  void SetBoard(const BitBoard &board);

  void Start();
  void Abort();
 private:
  BitBoard m_board;
  std::unique_ptr<Search> m_search;
  std::unique_ptr<std::thread> m_thread;
};
