#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <atomic>
#include <thread>

#include "score.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

class Computer{
 public:
  Computer(Color color);
  ~Computer();

  void SetBoard(const BitBoard &board);

  void Start();

  void Wait();
  bool IsReady();

  Turn Get();

  Turn Work();

 private:
  TTable m_table;
  BitBoard m_board;
  Color m_color;
};

#endif
