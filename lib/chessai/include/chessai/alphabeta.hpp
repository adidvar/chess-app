#pragma once

#include <chessai/score.hpp>
#include <chesscore/bitboard.hpp>
#include <chrono>

#include "search.hpp"

class Search : public SearchContext {
 public:
  Search(const BitBoard &board, Color color, SearchSettings settings = {})
      : SearchContext(board, color, settings) {}

  void iterativeSearch(Score a = Score::min(), Score b = Score::max()) {
    std::pair<Score, Turn> result = {{}, {}};
    int depth = 1;

    using clock = std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    auto begin = clock::now();

    for (int depth = 1; depth <= m_settings.serch_depth; depth++) {
      std::pair<Score, Turn> temp_result;
      if (m_color == Color::White)
        temp_result = searchTurn<true>(depth);
      else
        temp_result = searchTurn<false>(depth);

      if (!temp_result.first.isValid() || !temp_result.first.isValid()) break;

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

  template <bool min_max>
  Score search(ThreadContext *context, Score alpha, Score beta, int depth) {
    m_counter.newPosition();

    if (m_stop_flag) return Score{};

    if (depth == 0) return Score::getStaticValue(context->top());

    Turn turns[216];  // turn storage

    bool in_check;
    int turns_size =
        context->top().getTurns(context->top().getCurrentSide(), turns,
                                in_check);  // generate turns

    if (turns_size == 0) {
      if (in_check) return Score::checkMate(depth);
      return Score::tie();
    }

    auto best_score = min_max ? Score::min() : Score::max();

    for (int i = 0; i < turns_size; ++i) {
      context->applyTurn(turns[i]);
      Score score = search<!min_max>(context, alpha, beta, depth - 1);
      context->undoTurn();

      if constexpr (min_max) {
        if (score > best_score) {
          best_score = score;
          if (score > alpha) alpha = score;
        }
      } else {
        if (score < best_score) {
          best_score = score;
          if (score < beta) beta = score;
        }
      }
      if (beta <= alpha) break;
    }

    if (m_stop_flag) return Score{};

    return best_score;
  }

  template <bool min_max>
  Score searchThread(Turn turn, Score alpha, Score beta, int depth) {
    auto context = &m_pool[std::this_thread::get_id()];
    context->m_boards_stack.emplace(m_board, turn);
    return search<min_max>(context, alpha, beta, depth);
    context->undoTurn();
  }

  template <bool min_max>
  std::pair<Score, Turn> searchTurn(int depth) {
    if (m_stop_flag) return {Score(), Turn()};

    Turn turns[216];  // turn storage

    bool in_check;
    int turns_size = m_board.getTurns(m_board.getCurrentSide(), turns,
                                      in_check);  // generate turns
    if (turns_size == 0) {
      if (in_check) return {Score::checkMate(depth), Turn()};
      return {Score::tie(), Turn()};
    }

    Score alpha = (Score::ScoreType)Score::min(),
          beta = (Score::ScoreType)Score::max();

    Score best_score = min_max ? Score::min() : Score::max();
    Turn best_turn = {};
    std::mutex mutex;

    for (int i = 0; i < turns_size; ++i) {
      auto score = searchThread<!min_max>(turns[i], alpha, beta, depth - 1);

      if constexpr (min_max) {
        if (score > best_score) {
          best_score = score;
          best_turn = turns[i];
          if (score > alpha) alpha = score;
        }
      } else {
        if (score < best_score) {
          best_score = score;
          best_turn = turns[i];
          if (score < beta) beta = score;
        }
      }
      if (beta <= alpha) break;
    }

    if (m_stop_flag) return {Score{}, Turn{}};

    return {best_score, best_turn};
  }
};
