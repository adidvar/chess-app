#ifndef ORDERING_HPP
#define ORDERING_HPP

#include <algorithm>

#include "bftable.hpp"
#include "ttable.hpp"

inline void ReOrder(const BitBoard &board, std::vector<BitBoardTuple> &vector,
                    const BFTable &bftable, TTable *ttable) {
  for (auto &elem : vector) {
    auto from_pos = elem.turn.from();
    auto to_pos = elem.turn.to();

    auto from_figure = board.GetFigure(from_pos);
    auto to_figure = board.GetFigure(to_pos);

    auto from_price = Evaluate::FigurePrice(from_figure);
    auto to_price = Evaluate::FigurePrice(to_figure);

    /*
    bool founded = false;
    auto hashed = ttable.Search(elem.hash, founded);
    auto hashed_value = -hashed->value;

*/
    //   elem.priority = (founded ? hashed_value : 0).Value() / 1;
    elem.priority = 0;

    elem.priority += from_price / 10;
    elem.priority += to_price;

    elem.priority += bftable.Get(elem.turn) / 200;
  }
  std::sort(vector.rbegin(), vector.rend(),
            [&](const BitBoardTuple &t1, const BitBoardTuple &t2) {
              return t1.priority < t2.priority;
            });
}

inline void ReOrderQ(const BitBoard &board, std::vector<BitBoardTuple> &vector,
                     const BFTable &bftable, TTable &ttable) {
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

    if (founded && hashed->type == SearchElement::PV) elem.priority += 1000000;

    elem.priority += from_price / 10;
    elem.priority += to_price;

    elem.priority += bftable.Get(elem.turn) / 10;
  }
  std::sort(vector.rbegin(), vector.rend(),
            [&](const BitBoardTuple &t1, const BitBoardTuple &t2) {
              return t1.priority < t2.priority;
            });
}

#endif