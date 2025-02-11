#pragma once

#include <chessai/alphabeta.hpp>
#include <chesscore/figures.hpp>
#include <thread>

class Computer{
 public:
  Computer();
  ~Computer();

  void SetBoard(const BitBoard &board);

  void Start(SearchSettings settings);
  void Abort();

 private:
  std::unique_ptr<Search> m_search;
  std::unique_ptr<std::thread> m_thread;
};
