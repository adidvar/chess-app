#pragma once

#include <chesscore/bitboard.hpp>
#include <stack>
#include <thread>
#include <unordered_map>

#include "eventcounter.hpp"
#include "searchsettings.hpp"

class ThreadContext {
 public:
  std::stack<BitBoard> m_boards_stack;
  const BitBoard &top() { return m_boards_stack.top(); }
  void undoTurn() { m_boards_stack.pop(); }
  void applyTurn(Turn turn) {
    m_boards_stack.emplace(m_boards_stack.top(), turn);
  }
};

class SearchContext {
 public:
  // new search
  SearchContext(SearchSettings settings) : m_settings(settings) {}

  // base
  const SearchSettings m_settings;
  // stop
  StopFlag m_stop_flag;
  // feedback
  EventCounter m_counter;
  // counter
  std::unordered_map<std::thread::id, ThreadContext> m_pool;
  // thread contexts
};
