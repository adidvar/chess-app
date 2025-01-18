#pragma once

#include "alphabeta.hpp"

class ItDeepening : public AlphaBeta {
 public:
  ItDeepening(const BitBoard &board, Color color) : AlphaBeta(board, color) {}

  Score GetValue(int max_depth, Score a = Score::min(),
                 Score b = Score::max()) {
    m_last_depth = 0;
    Score result{};
    Turn turn{};
    try {
      for (int depth = 1; depth <= max_depth; depth++) {
        result = AlphaBeta::GetValue(depth, a, b);
        turn = AlphaBeta::GetTurn();
        //***TODO
        // if (GetTTable() != nullptr) GetTTable()->ClearNoTriggered();
        m_last_depth = depth;
      }
    } catch (SearchExitException exception) {
      m_last_turn = turn;
    }
    return result;
  }

  int GetLastDepth() const { return m_last_depth; }
};
