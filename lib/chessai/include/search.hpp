#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <atomic>

#include "bitboard.hpp"
#include "figures.hpp"
#include "searchsettings.hpp"
#include "statistics.hpp"

struct SearchExitException {};

class Search {
 public:
  Search(const BitBoard &board, Color color)
      : m_color(color), m_board(board), m_settings(board) {}

  [[nodiscard]] std::atomic_bool *GetStopFlag() const { return m_stop_flag; }
  void SetStopFlag(std::atomic_bool *stop_flag) { m_stop_flag = stop_flag; }

  void CheckStopFlag() {
    if ((m_stop_flag != nullptr) && (m_stop_flag->load()))
      throw SearchExitException{};
  }

  [[nodiscard]] Statistics GetStatistics() const { return m_stat; }
  [[nodiscard]] Color GetColor() const { return m_color; }
  [[nodiscard]] BitBoard GetBoard() const { return m_board; }
  [[nodiscard]] SearchSettings GetSearchSettings() const { return m_settings; }
  [[nodiscard]] Statistics &GetStatistics() { return m_stat; }

 private:
  std::atomic_bool *m_stop_flag = nullptr;
  SearchSettings m_settings;
  Statistics m_stat;
  const Color m_color;
  const BitBoard m_board;
};

#endif
