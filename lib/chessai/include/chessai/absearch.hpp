#pragma once

#include <chessai/score.hpp>
#include <chesscore/bitboard.hpp>

#include "ordering.hpp"
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

  sortMoves(context, turns, size, depth);

  auto bscore = mode ? Score::min() : Score::max();

  for (int i = 0; i < size; ++i) {
    context->applyTurn(turns[i]);
    Score score = alphaBeta<!mode>(context, alpha, beta, depth - 1);
    context->undoTurn();

    if constexpr (mode) {
      if (score > bscore) {
        bscore = score;
        if (score > alpha) {
          alpha = score;
          context->incTurn(turns[i], depth);
        }
      }
    } else {
      if (score < bscore) {
        bscore = score;
        if (score < beta) {
          beta = score;
          context->incTurn(turns[i], depth);
        }
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
  thread_context->initBoard(board, depth, 1);
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

  auto thread_context = context->getContextInstance();

  thread_context->initBoard(board, depth, 0);

  bool check;
  int size = board.getTurns(board.side(), turns, check);
  sortMoves(thread_context, turns, size, depth);

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
        if (score > alpha) {
          alpha = score;
          thread_context->incTurn(turns[i], depth);
        }
      }
    } else {
      if (score < bscore) {
        bscore = score;
        bturn = turns[i];
        if (score < beta) {
          beta = score;
          thread_context->incTurn(turns[i], depth);
        }
      }
    }
    if (beta <= alpha) break;
  }

  thread_context->clearBoard();

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
