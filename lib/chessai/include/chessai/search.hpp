#pragma once

#include <chesscore/bitboard.hpp>
#include <stack>
#include <thread>
#include <unordered_map>

#include "eventcounter.hpp"
#include "searchsettings.hpp"

class ThreadContext {
 public:
  ThreadContext(Color color, const StopFlag &flag, EventCounter &counter)
      : color(color), stop(flag), counter(counter) {}

  const BitBoard &top() {
    return boards_stack.top();
  }
  void initBoard(const BitBoard &board) {
    boards_stack.emplace(board);
  }
  void clearBoard() {
    while (!boards_stack.empty()) undoTurn();
  }
  void undoTurn() {
    boards_stack.pop();
  }
  void applyTurn(Turn turn) {
    boards_stack.emplace(boards_stack.top(), turn);
  }

  const Color color;
  const StopFlag &stop;
  EventCounter &counter;

 private:
  std::stack<BitBoard> boards_stack;
};

class SearchContext {
 public:
  SearchContext(SearchSettings settings) : settings(settings) {}
  const SearchSettings settings;

  ThreadContext *getContextInstance() {
    auto id = std::this_thread::get_id();
    if (!pool.contains(id))
      pool.insert({id, ThreadContext(settings.board.side(), stop, counter)});
    return &pool.at(id);
  }

  StopFlag stop;
  EventCounter counter;

 private:
  std::unordered_map<std::thread::id, ThreadContext> pool;
};
