#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <atomic>

#include "bitboard.hpp"
#include "figures.hpp"
#include "searchsettings.hpp"

struct SearchExitException {};

class Search {
 public:
  Search(const BitBoard &board, Color color)
      : m_color(color), m_board(board), m_settings() {}

  [[nodiscard]] std::atomic_bool *GetStopFlag() const { return m_stop_flag; }
  void SetStopFlag(std::atomic_bool *stop_flag) { m_stop_flag = stop_flag; }

  void CheckStopFlag() {
    if ((m_stop_flag != nullptr) && (m_stop_flag->load()))
      throw SearchExitException{};
  }

  [[nodiscard]] Color GetColor() const { return m_color; }
  [[nodiscard]] BitBoard GetBoard() const { return m_board; }
  [[nodiscard]] SearchSettings GetSearchSettings() const { return m_settings; }

  [[nodiscard]] int GetMainNode() const { return generation_count_; }
  [[nodiscard]] int GetEndNode() const { return approximation_count_; }
  [[nodiscard]] int GetExtraNode() const { return extra_node_; }

  void ClearStatistics() {
    generation_count_ = 0;
    approximation_count_ = 0;
    extra_node_ = 0;
  }

 protected:
  void MainNode() { generation_count_++; }
  void EndNode() { approximation_count_++; }
  void ExtraNode() { extra_node_++; }

 private:
  std::atomic_bool *m_stop_flag = nullptr;
  SearchSettings m_settings;
  const Color m_color;
  const BitBoard m_board;

  int generation_count_ = 0;
  int approximation_count_ = 0;
  int extra_node_ = 0;
};

#endif
