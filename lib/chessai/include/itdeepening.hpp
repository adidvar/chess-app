#ifndef ITDEEPENING_HPP
#define ITDEEPENING_HPP

#include <atomic>

#include "alphabeta.hpp"
#include "bitboard.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

class ItDeepening : public AlphaBeta {
 public:
  ItDeepening(const BitBoard &board, Color color) : AlphaBeta(board, color) {}

  Score GetValue(int max_depth, Score a = T::Min(), Score b = T::Max()) {
    m_last_depth = 0;
    Score result{};
    try {
      for (int depth = 1; depth <= max_depth; depth++) {
        result = AlphaBeta::GetValue(depth, a, b);
        if (GetTTable() != nullptr) GetTTable()->ClearNoTriggered();
        m_last_depth = depth;
      }
    } catch (SearchExitException exception) {
    }
    return result;
  }

  int GetLastDepth() const { return m_last_depth; }

 private:
  int m_last_depth = 0;
};

#endif
