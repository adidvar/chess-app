#ifndef ITDEEPENING_HPP
#define ITDEEPENING_HPP

#include <atomic>

#include "alphabeta.hpp"
#include "awindow.hpp"
#include "bitboard.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

template <class Search>
class ItDeepening {
  using T = Score;

 public:
  ItDeepening(Color color) : m_color(color) {}

  T GetValue(const BitBoard &board, int max_depth, T a = T::Min(),
             T b = T::Max()) {
    Search ab(board, m_color);
    ab.SetStopFlag(m_stop_flag);
    ab.SetTTable(m_ttable);
    T result = T();
    try {
      for (int depth = 1; depth <= max_depth; depth++) {
        Score value = ab.GetValue(depth);
        result = value;
        m_last_depth = depth;
      }
    } catch (SearchExitException exception) {
    }
    m_stat = ab.GetStatistics();
    return result;
  }

  Turn GetTurn(const BitBoard &board, int max_depth) {
    Search ab(board, m_color);
    ab.SetTTable(m_ttable);
    auto result = ab.GetTurn(max_depth);
    m_stat = ab.GetStatistics();
    return result;
  }

  std::vector<Turn> FindPV(BitBoard board, int max_depth) {
    Search ab(board, m_color);
    ab.SetTTable(m_ttable);
    std::vector<Turn> result;
    result = ab.FindPV(max_depth);
    m_stat = ab.GetStatistics();
    return result;
  }

  Statistics GetStatistics() const { return m_stat; };

  TTable *GetTTable() const { return m_ttable; };
  void SetTTable(TTable *newTtable) { m_ttable = newTtable; };

  std::atomic_bool *GetStopFlag() const { return m_stop_flag; };
  void SetStopFlag(std::atomic_bool *Stop_flag) { m_stop_flag = Stop_flag; };

  int GetLastDepth() const { return m_last_depth; };

 private:
  int m_last_depth = 0;
  std::atomic_bool *m_stop_flag = nullptr;
  Color m_color;
  Statistics m_stat;
  TTable *m_ttable = nullptr;
};

#endif
