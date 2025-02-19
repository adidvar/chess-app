#pragma once

#include <chessai/score.hpp>
#include <chesscore/bitboard.hpp>

#include "search.hpp"

template <bool mode>
inline Score alphaBeta(ThreadContext *context, Score alpha, Score beta,
                       int depth) {
  context->counter.newPosition();

  if (context->stop) return Score{};

  if (depth == 0) return Score::getStaticValue(context->top());

  Turn turns[216];

  bool check;
  int size = context->top().getTurns(context->top().side(), turns, check);

  if (size == 0) {
    if (check) return Score::checkMate(context->top().side(), depth);
    return Score::tie();
  }

  auto bscore = mode ? Score::min() : Score::max();

  for (int i = 0; i < size; ++i) {
    context->applyTurn(turns[i]);
    Score score = alphaBeta<!mode>(context, alpha, beta, depth - 1);
    context->undoTurn();

    if constexpr (mode) {
      if (score > bscore) {
        bscore = score;
        if (score > alpha) alpha = score;
      }
    } else {
      if (score < bscore) {
        bscore = score;
        if (score < beta) beta = score;
      }
    }
    if (beta <= alpha) break;
  }

  if (context->stop) return Score{};

  return bscore;
}

template <typename Lambda>
inline void alphaBetaWrapper(const BitBoard &board, SearchContext *context,
                             Score alpha, Score beta, int depth,
                             Lambda lambda) {
  auto thread_context = context->getContextInstance();
  thread_context->initBoard(board);
  auto score = (board.side() == Color::White)
                   ? alphaBeta<true>(thread_context, alpha, beta, depth)
                   : alphaBeta<false>(thread_context, alpha, beta, depth);
  thread_context->clearBoard();
  if (score.isValid()) lambda(score);
}

inline std::pair<Score, Turn> alphaBetaTurn(const BitBoard &board,
                                            SearchContext *context, Score alpha,
                                            Score beta, int depth) {
  Turn turns[216];

  bool check;
  int size = board.getTurns(board.side(), turns, check);

  if (size == 0) {
    if (check) return {Score::checkMate(board.side(), depth), Turn()};
    return {Score::tie(), Turn()};
  }

  bool mode = (board.side() == Color::White);
  Score bscore = mode ? Score::min() : Score::max();
  Turn bturn = {};

  for (int i = 0; i < size; ++i) {
    Score score;
    alphaBetaWrapper(BitBoard(board, turns[i]), context, alpha, beta, depth - 1,
                     [&score](Score temp) { score = temp; });
    if (!score.isValid()) return {{}, {}};

    if (mode) {
      if (score > bscore) {
        bscore = score;
        bturn = turns[i];
        if (score > alpha) alpha = score;
      }
    } else {
      if (score < bscore) {
        bscore = score;
        bturn = turns[i];
        if (score < beta) beta = score;
      }
    }
    if (beta <= alpha) break;
  }

  return {bscore, bturn};
}

inline void iterativeSearch(SearchContext *context) {
  const SearchSettings &settings = context->settings;
  std::pair<Score, Turn> result = {{}, {}};
  int depth = 1;

  using clock = std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;

  auto begin = clock::now();

  for (int depth = 1; depth <= context->settings.depth; depth++) {
    std::pair<Score, Turn> temp = alphaBetaTurn(
        settings.board, context, Score::min(), Score::max(), depth);
    auto elapsed =
        duration_cast<milliseconds>(clock::now() - begin).count() + 1;

    if (!temp.first.isValid() || !temp.second.isValid()) break;

    result = temp;
    settings.feedback->setDepth(depth);
    settings.feedback->setSelDepth(depth);
    settings.feedback->setScore(temp.first.toString(depth));
    settings.feedback->setPVLine({temp.second});
    settings.feedback->setNodesSearched(context->counter.getPosition());
    settings.feedback->setTimeElapsed(elapsed);
    settings.feedback->setNPS(context->counter.getPosition() /
                              (elapsed / 1000.0));
    settings.feedback->flush();

    if (context->counter.getPosition() > settings.nodes) break;  // nodes
    if (elapsed > settings.time) break;                          // time
  }
  settings.feedback->sendBestMove(result.second);
}

// search function
/*
alphaBeta --search for value
alphaBetaWrapper --search for value (non tempalate, with BitBoard parameter)
alphaBetaWrapper --multithread search for value (non tempalate, with
BitBoard parameter) alphaBetaTurn -- multithread search for turn (calls
alphaBetaWrapper, nontemplate) findPV -- search for pv recursively, with
alphaBetaTurn;
-----------------------------------------------------------------------------------------enter-points--
iterativeSearch --multithread search for turn (non tempalate, with BitBoard
parameter) iterativeMate --multithread search for  (non tempalate, with
BitBoard parameter)
-------------------------------------------------------------------------------------------------------

  template <bool mode>
  Score alphaBeta(ThreadContext *context, Score alpha, Score beta, int
depth)

  template<typename Lambda>
  void alphaBetaWrapper(const BitBoard &board, SearchContext *context, Score
alpha, Score beta, int depth, Lambda labmda) { }

  template<typename Lambda>
  void alphaBetaWrapper(const BitBoard &board, SearchContext *context, Score
alpha, Score beta, int depth, Lambda labmda) { }

  Turn alphaBetaTurn(const BitBoard &board, SearchContext *context, Score
alpha, Score beta, int depth);


  void iterativeSearch(SearchContext *context); //output to ifeedback
  void iterativeMate(SearchContext *context); //output to ifeedback

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

    auto elapsed = duration_cast<milliseconds>(clock::now() -
begin).count();

    result = temp_result;
    m_settings.feedback->setDepth(depth);
    m_settings.feedback->setSelDepth(depth);
    m_settings.feedback->setScore(temp_result.first.toString(depth));
    m_settings.feedback->setPVLine(searchPV(temp_result, depth));
    m_settings.feedback->setNodesSearched(m_counter.getPosition());
    m_settings.feedback->setTimeElapsed(elapsed);
    m_settings.feedback->setNPS(m_counter.getPosition() / (elapsed /
1000.0)); m_settings.feedback->flush();

    if (m_counter.getPosition() > m_settings.nodes) break;  // nodes limit
    if (elapsed > m_settings.time) break;                   // time soft
limit
  }
  m_settings.feedback->sendBestMove(result.second);
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
*/
