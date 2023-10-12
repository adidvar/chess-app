#ifndef ORDERING_HPP
#define ORDERING_HPP

#include <algorithm>

#include "bftable.hpp"
#include "ttable.hpp"

inline void ReOrder(const BitBoard &board, std::vector<BitBoardTuple> &vector,
                    Evaluate a, Evaluate b, const BFTable &bftable,
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

    const SearchElement *element = nullptr;
    bool found = false;
    if (ttable != nullptr && (depthmax - depthleft) <= 2) {
      element = ttable->Search(elem.hash, found);
    }
    if (elem.turn == pv && !(pv == Turn())) {
      elem.priority.type = PV;
      elem.priority.index = 0;
    } else if (found && -element->value > a) {
      elem.priority.type = Hashed;
      elem.priority.index = -element->value.Value();
    } else if (to_figure != Figure::kEmpty) {
      auto delta_price =
          Evaluate::FigurePrice(to_figure) - Evaluate::FigurePrice(from_figure);
      elem.priority.type = delta_price >= 0 ? PositiveAttack : NegativeAttack;
      elem.priority.index =
          delta_price + 200 * bftable.Get(elem.turn, depthleft);
    } else {
      elem.priority.type = NormalMoves;
      elem.priority.index = Evaluate::FigurePrice(from_figure) +
                            bftable.Get(elem.turn, depthleft) * 200;
    }
  }
  std::sort(vector.rbegin(), vector.rend());
}

inline void ReOrderQ(const BitBoard &board, std::vector<BitBoardTuple> &vector,
                     const BFTable &bftable, TTable &ttable) {
  /*
  for (auto &elem : vector) {
    auto from_pos = elem.turn.from();
    auto to_pos = elem.turn.to();

    auto from_figure = board.GetFigure(from_pos);
    auto to_figure = board.GetFigure(to_pos);

    auto from_price = Evaluate::FigurePrice(from_figure);
    auto to_price = Evaluate::FigurePrice(to_figure);

    bool founded = false;
    auto hashed = ttable.Search(elem.hash, founded);

    elem.priority = 0;

    if (founded && hashed->type == SearchElement::PV) elem.priority +=
  1000000;

    elem.priority += from_price / 10;
    elem.priority += to_price;

    elem.priority += bftable.Get(elem.turn) / 10;
  }
  std::sort(vector.rbegin(), vector.rend(),
            [&](const BitBoardTuple &t1, const BitBoardTuple &t2) {
              return t1.priority < t2.priority;
            });
*/
}

#endif
