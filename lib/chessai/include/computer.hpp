#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <thread>

#include "evaluate.hpp"
#include "match.hpp"
#include "statistics.hpp"

class Computer{
 public:
  Computer();
  ~Computer();

  // void NewGame();
  // void Position(const Match &match);

  // void Go();

  // void Wait();
  // bool Ready();

  // Turn GetBestTurn();

  // Statistics GetStatistics();

 private:
  // Match match_;

  // Statistics stat_;

  // std::future<Turn> turn_future_;
};

#endif
