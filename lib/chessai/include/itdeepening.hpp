#ifndef ITDEEPENING_HPP
#define ITDEEPENING_HPP

#include <atomic>

#include "alphabeta.hpp"
#include "bitboard.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

template <typename T>
class ItDeepening {
 public:
  ItDeepening(Color color, std::atomic_bool &stop_flag)
      : m_color(color), m_stop_flag(stop_flag) {}

  T GetValue(const BitBoard &board, int time, T a = T::Min(), T b = T::Max()) {
    AlphaBeta<T> ab(m_color, m_stop_flag);
    ab.SetTTable(m_ttable);
    T result = T();
    try {
      for (int depth = 1;; depth++) result = ab.GetValue(board, depth);
    } catch (SearchExitException exception) {
      return result;
    }
    return result;
  }

  Turn GetTurn(const BitBoard &board) {
    AlphaBeta<T> ab(m_color, m_stop_flag);
    ab.SetTTable(m_ttable);
    Turn result = Turn();
    try {
      for (int depth = 1;; depth++) result = ab.GetTurn(board, depth);
    } catch (SearchExitException exception) {
      return result;
    }
    return result;
  }

  std::vector<Turn> FindPV(BitBoard board) {
    AlphaBeta<T> ab(m_color, m_stop_flag);
    ab.SetTTable(&m_ttable);
    std::vector<Turn> result;
    try {
      for (int depth = 1;; depth++) result = ab.FindPV(board, depth);
    } catch (SearchExitException exception) {
      return result;
    }
    return result;
  }

  Statistics GetStatistics() const { return m_stat; };

 private:
  std::atomic_bool &m_stop_flag;
  const Color m_color;
  Statistics m_stat;
  TTable m_ttable;
};

#endif
