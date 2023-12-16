#ifndef ORDERING_HPP
#define ORDERING_HPP

#include <algorithm>

#include "bftable.hpp"
#include "bitboardtuple.hpp"
#include "ttable.hpp"

inline void ReOrder(const BitBoard &board, std::vector<BitBoardTuple> &vector,
                    Score a, Score b, const BFTable &bftable,
                    const TTable *const ttable, int depthleft, int depthmax,
                    Turn pv) {
  constexpr int history_scale = 50;

  enum TurnTypes {
    PV = 6,
    Hashed = 5,
    PositiveAttack = 4,
    KillerMoves = 3,
    NormalMoves = 2,
    NegativeAttack = 1,
  };

  for (auto &elem : vector) {
    auto from_pos = elem.turn.from();
    auto to_pos = elem.turn.to();

    auto from_figure = board.GetFigure(from_pos);
    auto to_figure = board.GetFigure(to_pos);

    Score hashed_value{};

    if (ttable != nullptr) {
      bool founded = false;
      const auto *node = ttable->Search(elem.hash, founded);
      hashed_value = node->value;
    }

    if (elem.turn == pv && pv != Turn()) {
      elem.priority.type = PV;
      elem.priority.index = 0;
    } else if (hashed_value != Score{}) {
      elem.priority.type = Hashed;
      elem.priority.index = hashed_value.Value();
    } else if (to_figure != Figure::kEmpty) {
      auto delta_price =
          Score::FigurePrice(to_figure) - Score::FigurePrice(from_figure);
      elem.priority.type = delta_price >= 0 ? PositiveAttack : NegativeAttack;
      elem.priority.index = delta_price;
    } else if (bftable.GetKiller(elem.turn, depthleft, depthmax)) {
      elem.priority.type = KillerMoves;
      elem.priority.index = Score::FigurePrice(from_figure) +
                            bftable.GetKiller(elem.turn, depthleft, depthmax) *
                                bftable.GetHistory(elem.turn);
    } else {
      elem.priority.type = NormalMoves;
      elem.priority.index = Score::FigurePrice(from_figure) +
                            bftable.GetHistory(elem.turn) * history_scale;
    }
  }
  std::sort(vector.rbegin(), vector.rend());
}

inline void BFTableReorderer(const BitBoard &board,
                             std::vector<BitBoardTuple> &vector,
                             const BFTable &bftable, int depthleft,
                             int depthmax, Turn pv) {
  constexpr int history_scale = 50;

  enum TurnTypes {
    PV = 5,
    PositiveAttack = 4,
    KillerMoves = 3,
    NormalMoves = 2,
    NegativeAttack = 1,
  };

  for (auto &elem : vector) {
    auto from_pos = elem.turn.from();
    auto to_pos = elem.turn.to();

    auto from_figure = board.GetFigure(from_pos);
    auto to_figure = board.GetFigure(to_pos);

    if (elem.turn == pv && pv != Turn()) {
      elem.priority.type = PV;
      elem.priority.index = 0;
    } else if (to_figure != Figure::kEmpty) {
      auto delta_price =
          Score::FigurePrice(to_figure) - Score::FigurePrice(from_figure);
      elem.priority.type = delta_price >= 0 ? PositiveAttack : NegativeAttack;
      elem.priority.index = delta_price;
    } else if (bftable.GetKiller(elem.turn, depthleft, depthmax)) {
      elem.priority.type = KillerMoves;
      elem.priority.index = Score::FigurePrice(from_figure) +
                            bftable.GetKiller(elem.turn, depthleft, depthmax) *
                                bftable.GetHistory(elem.turn);
    } else {
      elem.priority.type = NormalMoves;
      elem.priority.index = Score::FigurePrice(from_figure) +
                            bftable.GetHistory(elem.turn) * history_scale;
    }
  }
  std::sort(vector.rbegin(), vector.rend());
}
#endif
