#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <thread>

#include "evaluate.hpp"
#include "match.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

class Computer{
 public:
  Computer(Color color);
  ~Computer();

  void SetMatch(const Match &match);

  void Start();
  void Stop();

  std::vector<Turn> GetPV() const;
  Evaluate GetValue() const;
  Turn GetTurn() const;

  Statistics GetStatistics();
 private:
  Color m_color;
  Match m_match;
  TTable m_table;
  std::thread *m_thread = nullptr;

  std::vector<Turn> m_pv;
  Statistics m_stat;
  Evaluate m_value;
  Turn m_turn;
};

#endif
