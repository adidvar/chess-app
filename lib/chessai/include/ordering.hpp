#pragma once

/*
inline void sortMoves(const BitBoard &board, std::vector<BitBoard> &vector,
                    Score a, Score b, const HKTable &bftable,
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

    const TTableItem *element = nullptr;
    bool found = false;
    if (ttable != nullptr && (depthmax - depthleft) <= depthmax / 2) {
      element = ttable->Search(elem.hash, found);
    }

    if (elem.turn == pv && pv != Turn()) {
      elem.priority.type = PV;
      elem.priority.index = 0;
    } else if (found && -element->value >= a) {
      elem.priority.type = Hashed;
      elem.priority.index = -element->value;
    } else if (to_figure != Figure::kEmpty) {
      auto delta_price =
          Score::GetFigureScore(to_figure) - Score::GetFigureScore(from_figure);
      elem.priority.type =
          (int)delta_price >= 0 ? PositiveAttack : NegativeAttack;
      elem.priority.index = delta_price;
    } else if (bftable.GetKillerMoveCount(elem.turn, depthmax - depthleft)) {
      elem.priority.type = KillerMoves;
      elem.priority.index =
          (int)Score::GetFigureScore(from_figure) +
          bftable.GetKillerMoveCount(elem.turn, depthmax - depthleft) *
              bftable.GetHistoryCount(elem.turn);
    } else {
      elem.priority.type = NormalMoves;
      elem.priority.index = (int)Score::GetFigureScore(from_figure) +
                            bftable.GetHistoryCount(elem.turn) * history_scale;
    }
  }
  std::sort(vector.rbegin(), vector.rend());
}

inline void BFTableReorderer(const BitBoard &board,
                             std::vector<BitBoardTuple> &vector,
                             const HKTable &bftable, int depthleft,
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
          Score::GetFigureScore(to_figure) - Score::GetFigureScore(from_figure);
      elem.priority.type =
          (int)delta_price >= 0 ? PositiveAttack : NegativeAttack;
      elem.priority.index = delta_price;
    } else if (bftable.GetKillerMoveCount(elem.turn, depthmax - depthleft)) {
      elem.priority.type = KillerMoves;
      elem.priority.index =
          (int)Score::GetFigureScore(from_figure) +
          bftable.GetKillerMoveCount(elem.turn, depthmax - depthleft) *
              bftable.GetHistoryCount(elem.turn);
    } else {
      elem.priority.type = NormalMoves;
      elem.priority.index = (int)Score::GetFigureScore(from_figure) +
                            bftable.GetHistoryCount(elem.turn) * history_scale;
    }
  }
  std::sort(vector.rbegin(), vector.rend());
}
*/
