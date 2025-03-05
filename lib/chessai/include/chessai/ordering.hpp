#pragma once

#include <algorithm>
#include <vector>

#include "score.hpp"
#include "search.hpp"

enum class TurnTypes : int32_t {
  PV = 7,
  Hashed = 6,
  PositiveAttack = 5,
  KillerMoves = 4,
  NormalMoves = 3,
  NormalMovesHistory = 2,
  NegativeAttack = 1,
};

inline auto mergePriority(TurnTypes major, int32_t minor) {
  return (int64_t)major * 4294967296LL + minor;
}

inline void sortMoves(ThreadContext *context, Turn *turns, int count, int depth,
                      const std::vector<Turn> &pv = {}) {
  std::pair<int64_t, Turn> temp[216];

  for (int i = 0; i < count; i++) {
    if (turns[i].isAdvanced()) {  // attacks
      auto see = Score::getSEE(context->top(), turns[i]);
      // @todo add negative attacks after improved SEE
      temp[i].first = mergePriority(TurnTypes::PositiveAttack, see);
    } else if (std::find(pv.begin(), pv.end(), turns[i]) !=
               pv.end()) {  // normal moves
      temp[i].first = mergePriority(TurnTypes::PV, 0);
    } else {  // normal moves
      auto killer = context->getK(turns[i], depth);
      if (killer == 0)
        temp[i].first = mergePriority(TurnTypes::NormalMovesHistory,
                                      context->getH(turns[i]));
      else
        temp[i].first = mergePriority(TurnTypes::NormalMoves, killer);
    }

    temp[i].second = turns[i];
  }

  std::sort(temp, temp + count,
            [](auto p1, auto p2) { return p1.first > p2.first; });

=======

#include "search.hpp"

enum class TurnTypes : int32_t {
  PV = 6,
  Hashed = 5,
  PositiveAttack = 4,
  KillerMoves = 3,
  NormalMoves = 2,
  NegativeAttack = 1,
};

inline auto mergePriority(TurnTypes major, int32_t minor) {
  return (int64_t)major * 4294967296LL + minor;
}

inline void sortMoves(ThreadContext *context, Turn *turns, int count,
                      int depth) {
  constexpr int history_scale = 50;

  std::pair<int64_t, Turn> temp[216];

  for (int i = 0; i < count; i++) {
    auto from_pos = turns[i].from();
    auto to_pos = turns[i].to();

    auto from_figure = context->top().get(from_pos);
    auto to_figure = context->top().get(to_pos);

    if (to_figure != Figure::Empty) {  // attacks
      temp[i].first = mergePriority(TurnTypes::PositiveAttack, 0);
    } else {  // normal moves
      temp[i].first =
          mergePriority(TurnTypes::NormalMoves,
                        history_scale * context->getK(turns[i], depth) +
                            context->getH(turns[i]));
    }
    temp[i].second = turns[i];
  }

  std::sort(temp, temp + count,
            [](auto p1, auto p2) { return p1.first > p2.first; });

  for (int i = 0; i < count; i++) {
    turns[i] = temp[i].second;
  }
}
