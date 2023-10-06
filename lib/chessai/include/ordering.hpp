#ifndef ORDERING_HPP
#define ORDERING_HPP

#include <algorithm>

#include "bftable.hpp"
#include "ttable.hpp"

inline void ReOrder(const BitBoard &board, std::vector<BitBoardTuple> &vector,
                    const BFTable &bftable, TTable &ttable) {
  /*
  bitboard_hash_t hash;
  Evaluate price;
  for (auto elem : vector) {
    bool founded = false;
    auto cached = ttable.Search(elem.hash, founded);
    if (founded && cached->value > price) {
      price = cached->value;
      hash = cached->hash;
    }
  }
*/
  for (auto &elem : vector) {
    auto from_pos = elem.turn.from();
    auto to_pos = elem.turn.to();

    auto from_figure = board.GetFigure(from_pos);
    auto to_figure = board.GetFigure(to_pos);

    auto from_price = Evaluate::FigurePrice(from_figure);
    auto to_price = Evaluate::FigurePrice(to_figure);

    // bool founded = false;
    // auto hashed = ttable.Search(elem.hash, founded);
    // auto hashed_value = -hashed->value;

    // elem.priority = (founded ? hashed_value : -1000).Value();
    //  elem.priority = 0;

    elem.priority += from_price / 9;
    elem.priority += to_price;

    // elem.priority += bftable.Get(elem.turn) / 10;
  }
  std::sort(vector.rbegin(), vector.rend(),
            [&](const BitBoardTuple &t1, const BitBoardTuple &t2) {
              return t1.priority < t2.priority;
            });
}

inline void ReOrderQ(const BitBoard &board, std::vector<BitBoardTuple> &vector,
                     const BFTable &bftable, TTable &ttable) {
  /*
  bitboard_hash_t hash;
  Evaluate price;
  for (auto elem : vector) {
    bool founded = false;
    auto cached = ttable.Search(elem.hash, founded);
    if (founded && cached->value > price) {
      price = cached->value;
      hash = cached->hash;
    }
  }
*/
  for (auto &elem : vector) {
    auto from_pos = elem.turn.from();
    auto to_pos = elem.turn.to();

    auto from_figure = board.GetFigure(from_pos);
    auto to_figure = board.GetFigure(to_pos);

    auto from_price = Evaluate::FigurePrice(from_figure);
    auto to_price = Evaluate::FigurePrice(to_figure);

    // bool founded = false;
    // auto hashed = ttable.Search(elem.hash, founded);
    // auto hashed_value = -hashed->value;

    // elem.priority = (founded ? hashed_value : -1000).Value();
    //  elem.priority = 0;

    elem.priority += from_price / 9;
    elem.priority += to_price;

    elem.priority += bftable.Get(elem.turn) / 10;
  }
  std::sort(vector.rbegin(), vector.rend(),
            [&](const BitBoardTuple &t1, const BitBoardTuple &t2) {
              return t1.priority < t2.priority;
            });
}

#endif
