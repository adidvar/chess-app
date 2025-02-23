#pragma once

#include <algorithm>

#include "score.hpp"
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

inline void sortMoves(ThreadContext *context, Turn *turns, int count, int depth,
                      Turn pv = {}) {
  constexpr int history_scale = 50;

  std::pair<int64_t, Turn> temp[216];

  for (int i = 0; i < count; i++) {
    if (turns[i].isAttack()) {  // attacks
      auto see = Score::getSEE(context->top(), turns[i]);
      temp[i].first = mergePriority(see > Score{0} ? TurnTypes::PositiveAttack
                                                   : TurnTypes::NegativeAttack,
                                    see);
    } else if (pv.isValid() && turns[i] == pv) {  // normal moves
      temp[i].first = mergePriority(TurnTypes::PV, 0);
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
