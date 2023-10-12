#ifndef ORDERING_HPP
#define ORDERING_HPP

#include <algorithm>

#include "bftable.hpp"
#include "ttable.hpp"

inline void ReOrder(const BitBoard &board, std::vector<BitBoardTuple> &vector,
                    Evaluate a, Evaluate b, const BFTable &bftable,
                    const TTable *const ttable, int depthleft) {
  enum TurnTypes {
    HashedMovesWithACutting = 4,
    PositiveAttack = 3,
    NormalMoves = 2,
    NegativeAttack = 1,
  };

  /*
  Evaluate pv_value = Evaluate::Min();
  BitBoardTuple *pv = nullptr;

  if (ttable != nullptr) {
    for (auto &elem : vector) {
      bool founded = false;
      const auto *hashed = ttable->Search(elem.hash, founded);
      if (founded) {
        auto hashed_value = -hashed->value;
        if (hashed_value > pv_value && hashed_value > a &&
            elem.priority.type != HashedMovesWithACutting) {
          pv_value = hashed_value;
          pv = &elem;
        };
      }
    }
  }
  if (pv != nullptr) {
    pv->priority.type = HashedMovesWithACutting;
    pv->priority.index = pv_value.Value();
  };
*/

  for (auto &elem : vector) {
    auto from_pos = elem.turn.from();
    auto to_pos = elem.turn.to();

    auto from_figure = board.GetFigure(from_pos);
    auto to_figure = board.GetFigure(to_pos);

    bool founded = false;
    const SearchElement *hashed;
    Evaluate hashed_value;
    if (ttable != nullptr) {
      hashed = ttable->Search(elem.hash, founded);
      hashed_value = -hashed->value;
    }

    if (founded && hashed_value > a) {
      elem.priority.type = HashedMovesWithACutting;
      elem.priority.index = hashed_value.Value();
    } else if (to_figure != Figure::kEmpty) {
      auto delta_price =
          Evaluate::FigurePrice(to_figure) - Evaluate::FigurePrice(from_figure);
      elem.priority.type = delta_price >= 0 ? PositiveAttack : NegativeAttack;
      elem.priority.index = delta_price;
    } else {
      elem.priority.type = NormalMoves;
      elem.priority.index = Evaluate::FigurePrice(from_figure) +
                            bftable.Get(elem.turn, depthleft) * 200;
    }

    /*
    auto from_pos = elem.turn.from();
    auto to_pos = elem.turn.to();

    auto from_figure = board.GetFigure(from_pos);
    auto to_figure = board.GetFigure(to_pos);

    auto from_price = Evaluate::FigurePrice(from_figure);
    auto to_price = Evaluate::FigurePrice(to_figure);
*/

    //   elem.priority = (founded ? hashed_value : 0).Value() / 1;
    //   elem.priority = 0;

    //  elem.priority += from_price / 10;
    //  elem.priority += to_price;

    //  elem.priority += bftable.Get(elem.turn) / 200;
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
