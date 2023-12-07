#ifndef ORDERING_HPP
#define ORDERING_HPP

#include <algorithm>

#include "bftable.hpp"
#include "ttable.hpp"

inline void ReOrder(const BitBoard &board, std::vector<BitBoardTuple> &vector,
                    Score a, Score b, const BFTable &bftable,
                    const TTable *const ttable, int depthleft, int depthmax,
                    Turn pv) {
  enum TurnTypes {
    PV = 5,
    Hashed = 4,
    PositiveAttack = 3,
    NormalMoves = 2,
    NegativeAttack = 1,
  };

  for (auto &elem : vector) {
    auto from_pos = elem.turn.from();
    auto to_pos = elem.turn.to();

    auto from_figure = board.GetFigure(from_pos);
    auto to_figure = board.GetFigure(to_pos);

    const TTableItem *element = nullptr;
    bool found = false;
    if (ttable != nullptr && (depthmax - depthleft) <= depthmax / 2) {
      element = ttable->Search(elem.hash, found);
    }
    if (elem.turn == pv && !(pv == Turn())) {
      elem.priority.type = PV;
      elem.priority.index = 0;
    } else if (found) {
      elem.priority.type = Hashed;
      elem.priority.index = -element->value.Value();
    } else if (to_figure != Figure::kEmpty) {
      auto delta_price =
          Score::FigurePrice(to_figure) - Score::FigurePrice(from_figure);
      elem.priority.type = delta_price >= 0 ? PositiveAttack : NegativeAttack;
      elem.priority.index =
          delta_price + 200 * bftable.Get(elem.turn, depthleft, depthmax);
    } else {
      elem.priority.type = NormalMoves;
      elem.priority.index = Score::FigurePrice(from_figure) +
                            bftable.Get(elem.turn, depthleft, depthmax) * 200;
    }
  }
  std::sort(vector.rbegin(), vector.rend());
}

inline void BFTableReorderer(const BitBoard &board,
                             std::vector<BitBoardTuple> &vector,
                             const BFTable &bftable, int depthleft,
                             int depthmax, Turn pv) {
  constexpr int bftable_scale = 200;

  enum TurnTypes {
    PV = 4,
    PositiveAttack = 3,
    NormalMoves = 2,
    NegativeAttack = 1,
  };

  for (auto &elem : vector) {
    auto from_pos = elem.turn.from();
    auto to_pos = elem.turn.to();

    auto from_figure = board.GetFigure(from_pos);
    auto to_figure = board.GetFigure(to_pos);

    if (elem.turn == pv && !(pv == Turn())) {
      elem.priority.type = PV;
      elem.priority.index = 0;
    } else if (to_figure != Figure::kEmpty) {
      auto delta_price =
          Score::FigurePrice(to_figure) - Score::FigurePrice(from_figure);
      elem.priority.type = delta_price >= 0 ? PositiveAttack : NegativeAttack;
      elem.priority.index =
          delta_price +
          bftable_scale * bftable.Get(elem.turn, depthleft, depthmax);
    } else {
      elem.priority.type = NormalMoves;
      elem.priority.index =
          Score::FigurePrice(from_figure) +
          bftable.Get(elem.turn, depthleft, depthmax) * bftable_scale;
    }
  }
  std::sort(vector.rbegin(), vector.rend());
}
#endif
