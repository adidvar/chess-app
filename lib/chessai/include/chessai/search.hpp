#pragma once

#include <chesscore/bitboard.hpp>
#include <stack>
#include <thread>
#include <unordered_map>

#include "eventcounter.hpp"
#include "hktable.hpp"
#include "searchsettings.hpp"

class ThreadContext {
 public:
  ThreadContext(Color color, const StopFlag &flag, EventCounter &counter,
                HKTable &table)
      : color(color), stop(flag), counter(counter), table(table) {}

  const BitBoard &top() {
    return stack.top();
  }
  void initBoard(const BitBoard &board, unsigned depth_max,
                 unsigned depth_start) {
    this->depth_max = depth_max;
    this->depth_start = depth_start;
    stack.emplace(board);
  }
  void clearBoard() {
    while (!stack.empty()) undoTurn();
  }
  void undoTurn() {
    stack.pop();
  }
  void applyTurn(Turn turn) {
    stack.emplace(stack.top(), turn);
  }

  void incTurn(Turn turn, unsigned depth) {
    table.increment(turn, depth_start + this->depth_max - depth);
  }

  size_t getK(Turn turn, unsigned depth) {
    return table.getK(turn, depth_start + this->depth_max - depth);
  }

  size_t getH(Turn turn) {
    return table.getH(turn);
  }

  const Color color;
  const StopFlag &stop;
  EventCounter &counter;

 private:
  HKTable &table;
  std::stack<BitBoard> stack;
  unsigned depth_max;
  unsigned depth_start;
};

class SearchContext {
 public:
  SearchContext(SearchSettings settings) : settings(settings) {}
  const SearchSettings settings;

  ThreadContext *getContextInstance() {
    auto id = std::this_thread::get_id();
    if (!pool.contains(id))
      pool.insert(
          {id, ThreadContext(settings.board.side(), stop, counter, table)});
    return &pool.at(id);
  }

  StopFlag stop;
  EventCounter counter;
  HKTable table;

 private:
  std::unordered_map<std::thread::id, ThreadContext> pool;
};
