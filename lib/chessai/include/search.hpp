#pragma once

#include <chesscore/bitboard.hpp>
#include <memory>
#include <stack>

#include "eventcounter.hpp"
#include "ifeedback.hpp"
#include "searchsettings.hpp"

// search environment
class Search {
 public:
  // new search
  Search(const BitBoard &board, Color color, SearchSettings settings = {})
      : m_board(board),
        m_color(color),
        m_settings(settings),
        m_feedback(new IFeedBack()) {}

  // export old ttables and others
  void exportFrom(const Search &old_search);
  // export old ttables and new settings
  void exportFrom(const Search &old_search, SearchSettings settings);

 protected:
  // base
  const SearchSettings m_settings;
  const Color m_color;
  const BitBoard m_board;
  // counters
  StopFlag m_stop_flag;
  EventCounter m_counter;
  // bitboard stack
  std::stack<BitBoard> m_boards_stack;
  // feedback
  std::shared_ptr<IFeedBack> m_feedback;

  // stack manipulations
  const BitBoard &top() { return m_boards_stack.top(); }

  void undoTurn() { m_boards_stack.pop(); }

  void applyTurn(Turn turn) {
    m_boards_stack.emplace(m_boards_stack.top(), turn);
  }
};
