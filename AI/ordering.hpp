#ifndef ORDERING_HPP
#define ORDERING_HPP

#include <algorithm>

#include "evaluate.hpp"

template <typename T>
inline void ReOrder(const BitBoard &board, T begin, T end) {
  std::sort(begin, end, [&](const BitBoardTuple &t1, const BitBoardTuple &t2) {
    const static uint8_t order[] = {0, 1, 30, 30, 50, 90, 0};

    if (board.TestEmp(t1.turn.to()) != board.TestEmp(t2.turn.to()))
      return !board.TestEmp(t1.turn.to()) < !board.TestEmp(t2.turn.to());
    else if (!board.TestEmp(t1.turn.to()) && !board.TestEmp(t2.turn.to()))
      if (order[board.GetFigure(t1.turn.to())] !=
          order[board.GetFigure(t2.turn.to())])
        return order[board.GetFigure(t1.turn.to())] <
               order[board.GetFigure(t2.turn.to())];
      else
        return order[board.GetFigure(t1.turn.from())] <
               order[board.GetFigure(t2.turn.from())];
    else
      return order[board.GetFigure(t1.turn.from())] <
             order[board.GetFigure(t2.turn.from())];
  });
}

template <typename T>
inline void ReOrderQ(const BitBoard &board, T begin, T end) {
  std::sort(begin, end, [&](const BitBoardTuple &t1, const BitBoardTuple &t2) {
    const static uint8_t order[] = {0, 10, 30, 30, 50, 90, 0};

    if (order[board.GetFigure(t1.turn.to())] !=
        order[board.GetFigure(t2.turn.to())])
      return order[board.GetFigure(t1.turn.to())] <
             order[board.GetFigure(t2.turn.to())];
    else
      return order[board.GetFigure(t1.turn.from())] <
             order[board.GetFigure(t2.turn.from())];
  });
}

#endif
