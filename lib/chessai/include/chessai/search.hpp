#pragma once

#include <chesscore/bitboard.hpp>
#include <memory>
#include <stack>
#include <thread>
#include <unordered_map>

#include "eventcounter.hpp"
#include "ifeedback.hpp"
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
  SearchContext(const BitBoard &board, Color color,
                SearchSettings settings = {})
      : m_board(board),
        m_color(color),
        m_settings(settings),
        m_feedback(new IFeedBack()) {}

  // base
  const SearchSettings m_settings;
  const Color m_color;
  const BitBoard m_board;
  // stop
  StopFlag m_stop_flag;
  // feedback
  EventCounter m_counter;
  // counter
  std::shared_ptr<IFeedBack> m_feedback;
  // thread contexts
  std::unordered_map<std::thread::id, ThreadContext> m_pool;
};
