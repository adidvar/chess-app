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
  void Stop();

  std::vector<Turn> GetPV() const;
  Score GetValue() const;
  Turn GetTurn() const;
  int GetDepth() const;

  Statistics GetStatistics();
 private:
  Color m_color;
  BitBoard m_board;
  TTable m_table;
  std::atomic_bool m_stop_flag;
  std::thread *m_thread = nullptr;

  std::vector<Turn> m_pv;
  Statistics m_stat;
  Score m_value;
  int m_depth;
  Turn m_turn;
};

#endif
