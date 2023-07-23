#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <thread>

#include "match.hpp"
#include "openingsbase.hpp"
#include "position_rating.hpp"
#include "statistics.hpp"
#include "transpositiontable.hpp"

class Computer{
 public:
  Computer();
  ~Computer();

  void NewGame();
  void Position(const Match &match);

  void Go();

  void Wait();
  bool Ready();

  Turn GetBestTurn();

  Statistics GetStatistics();

 private:
  Match match_;

  Statistics stat_;

  std::future<Turn> turn_future_;
};

#endif
