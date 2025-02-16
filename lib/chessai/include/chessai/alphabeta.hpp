#pragma once

#include <chessai/score.hpp>
#include <chesscore/bitboard.hpp>
#include <chrono>

#include "search.hpp"

class Search : public SearchContext {
 public:
  Search(SearchSettings settings) : SearchContext(settings) {}

  void iterativeSearch() {
    Score a = Score::min(), b = Score::max();
    std::pair<Score, Turn> result = {{}, {}};
    int depth = 1;

    using clock = std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    auto begin = clock::now();

    for (int depth = 1; depth <= m_settings.depth; depth++) {
      std::pair<Score, Turn> temp_result;
      if (m_settings.board.getCurrentSide() == Color::White)
        temp_result = searchTurn<true>(depth);
      else
        temp_result = searchTurn<false>(depth);

      if (!temp_result.first.isValid() || !temp_result.first.isValid()) break;

      auto elapsed = duration_cast<milliseconds>(clock::now() - begin).count();

      result = temp_result;
      m_settings.feedback->setDepth(depth);
      m_settings.feedback->setSelDepth(depth);
      m_settings.feedback->setScore(temp_result.first.toString(depth));
      m_settings.feedback->setPVLine(searchPV(temp_result, depth));
      m_settings.feedback->setNodesSearched(m_counter.getPosition());
      m_settings.feedback->setTimeElapsed(elapsed);
      m_settings.feedback->setNPS(m_counter.getPosition() / (elapsed / 1000.0));
      m_settings.feedback->flush();

      if (m_counter.getPosition() > m_settings.nodes) break;  // nodes limit
      if (elapsed > m_settings.time) break;                   // time soft limit
    }
    m_settings.feedback->sendBestMove(result.second);
  }

  void mateSearch() {
    /*
    using clock = std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    auto begin = clock::now();

    unsigned founded_mate = -1;

    for (int depth = 1; depth <= m_settings.depth; depth++) {
      std::pair<Score, Turn> temp_result;
      if (m_settings.board.getCurrentSide() == Color::White)
        temp_result = searchTurn<true>(depth);
      else
        temp_result = searchTurn<false>(depth);

      if (!temp_result.first.isValid() || !temp_result.first.isValid()) break;

      auto elapsed = duration_cast<milliseconds>(clock::now() - begin).count();

      m_settings.feedback->setDepth(depth);
      m_settings.feedback->setSelDepth(depth);
      m_settings.feedback->setScore(temp_result.first.toString(depth));
      m_settings.feedback->setPVLine(searchPV(temp_result, depth));
      m_settings.feedback->setNodesSearched(m_counter.getPosition());
      m_settings.feedback->setTimeElapsed(elapsed);
      m_settings.feedback->setNPS(m_counter.getPosition() / (elapsed / 1000.0));
      m_settings.feedback->flushMate();

      if (m_counter.getPosition() > m_settings.nodes) break;  // nodes limit
      if (elapsed > m_settings.time) break;                   // time soft limit
    }
*/
  }

 private:
  std::vector<Turn> searchPV(std::pair<Score, Turn> pair, int depth) {
    std::vector<Turn> results{pair.second};
    BitBoard board = m_settings.board;
    board = board.executeTurn(board.getCurrentSide(), pair.second);
    depth--;
    for (int i = 0; i < depth; i++) {
    }

    return results;
  }

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
      if (in_check)
        return Score::checkMate(context->top().getCurrentSide(), depth);
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
    context->m_boards_stack.emplace(m_settings.board, turn);
    return search<min_max>(context, alpha, beta, depth);
    context->undoTurn();
  }

  template <bool min_max>
  std::pair<Score, Turn> searchTurn(int depth) {
    if (m_stop_flag) return {Score(), Turn()};

    Turn turns[216];  // turn storage

    bool in_check = false;
    int turns_size =
        m_settings.board.getTurns(m_settings.board.getCurrentSide(), turns,
                                  in_check);  // generate turns

    this->m_settings.feedback->sendDebug(m_settings.board.fen());
    for (int i = 0; i < turns_size; i++) {
      this->m_settings.feedback->sendDebug(std::to_string(i) + " " +
                                           turns[i].toString());
    }

    if (turns_size == 0) {
      if (in_check)
        return {Score::checkMate(m_settings.board.getCurrentSide(), depth),
                Turn()};
      return {Score::tie(), Turn()};
    }

    Score alpha = (Score::ScoreType)Score::min(),
          beta = (Score::ScoreType)Score::max();

    Score best_score = min_max ? Score::min() : Score::max();
    Turn best_turn = {};

    for (int i = 0; i < turns_size; ++i) {
      auto score = searchThread<!min_max>(turns[i], alpha, beta, depth - 1);
      // auto score = searchThread<!min_max>(turns[i], Score::min(),
      // Score::max(),
      //                                    depth - 1);
      // turns contains trash, wtf
      this->m_settings.feedback->sendDebug(std::to_string(i) + " " +
                                           turns[i].toString() + " " +
                                           std::to_string(score));

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
