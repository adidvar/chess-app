#pragma once

#include <chesscore/bitboard.hpp>
#include <chrono>
#include <score.hpp>

#include "search.hpp"

class AlphaBeta : public Search {
 public:
  AlphaBeta(const BitBoard &board, Color color, SearchSettings settings = {})
      : Search(board, color, settings) {}

  void iterativeSearch(Score a = Score::min(), Score b = Score::max()) {
    std::pair<Score, Turn> result = {{}, {}};
    int depth = 1;

    using clock = std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    auto begin = clock::now();

    for (int depth = 1; depth <= m_settings.serch_depth; depth++) {
      auto temp_result = search(depth);

      // if (!temp_result.first.isValid() || !temp_result.first.isValid())
      // break;

      result = temp_result;
      m_feedback->sendDepth(depth);
      m_feedback->sendScore(temp_result.first.toCentiPawns());
      m_feedback->sendTurn(temp_result.second);
      m_feedback->sendNodesSearched(m_counter.getPosition());
      m_feedback->sendTimeElapsed(
          duration_cast<milliseconds>(clock::now() - begin).count());
    }
    m_feedback->sendBestMove(result.second);
  }

 private:
  std::vector<Turn> searchPV() {}

  std::pair<Score, Turn> search(int depth, Score a = Score::min(),
                                Score b = Score::max()) {
    m_boards_stack.push(m_board);
    Score score;
    if (m_board.getCurrentSide() == Color::White)
      score = alphabeta<true, true>(a, b, depth);
    else
      score = alphabeta<false, true>(a, b, depth);
    m_boards_stack.pop();

    return {score, m_turn};
  }

  template <bool min_max, bool save_turn>
  Score alphabeta(Score alpha, Score beta, int depth) {
    m_counter.newPosition();

    if (m_stop_flag) return Score{};

    if (depth == 0) return Score::getStaticValue(top());

    Turn turns[216];  // turn storage

    bool in_check;
    int turns_size = top().getTurns(top().getCurrentSide(), turns,
                                    in_check);  // generate turns

    if (turns_size == 0) {
      if (in_check) return Score::checkMate(depth);
      return Score::tie();
    }

    Score best_score = min_max ? Score::min() : Score::max();
    Turn best_turn = {};

    for (int i = 0; i < turns_size; ++i) {
      applyTurn(turns[i]);
      Score score = alphabeta<!min_max, false>(alpha, beta, depth - 1);
      undoTurn();

      if constexpr (min_max) {
        if (score > best_score) {
          best_score = score;
          best_turn = turns[i];
          if (alpha < score) alpha = score;
        }
      } else {
        if (score < best_score) {
          best_score = score;
          best_turn = turns[i];
          if (beta > score) beta = score;
        }
      }
      if (beta <= alpha) break;
    }

    if constexpr (save_turn) m_turn = best_turn;

    return best_score;
  }

 private:
  Turn m_turn;
};
