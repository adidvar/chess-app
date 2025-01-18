#pragma once

#include <chesscore/bitboard.hpp>

#include "eventcounter.hpp"
#include "searchsettings.hpp"

// search environment
class Search {
 public:
  // new search
  Search(const BitBoard &board, Color color, SearchSettings settings = {});

  // export old ttables and others
  Search(const Search &old_search);

  // export old ttables and new settings
  Search(const Search &old_search, SearchSettings settings);

 private:
  // base
  const SearchSettings m_settings;
  const Color m_color;
  const BitBoard m_board;
  // counters
  StopFlag m_stop_flag;
  EventCounter m_counter;
};
