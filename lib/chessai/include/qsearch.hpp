#ifndef QSEARCH_HPP
#define QSEARCH_HPP

#include <algorithm>

#include "bitboard.hpp"
#include "bitboardtuple.hpp"
#include "boarditerator.hpp"
#include "evaluate.hpp"
#include "search.hpp"
#include "statistics.hpp"

inline void QuiescenceSearchOrdering(const BitBoard &board,
                                     std::vector<BitBoardTuple> &vector) {
  enum TurnTypes {
    PositiveAttack = 1,
    NegativeAttack = 0,
  };

  for (auto &elem : vector) {
    auto from_pos = elem.turn.from();
    auto to_pos = elem.turn.to();

    auto from_figure = board.GetFigure(from_pos);
    auto to_figure = board.GetFigure(to_pos);

    auto delta_price =
        Score::FigurePrice(to_figure) - Score::FigurePrice(from_figure);
    elem.priority.type = delta_price >= 0 ? PositiveAttack : NegativeAttack;
    elem.priority.index = delta_price;
  }
  std::sort(vector.rbegin(), vector.rend());
}

class QSearch : public Search, private ChessTreeHash {
  using T = Score;

  inline const static Score kBIG_DELTA{
      Score::FigurePrice(Figure::kPawn) +
      Score::FigurePrice(Figure::kQueen)};  // queen value

 public:
  QSearch(Color color) : Search(color) {}

  T QuiescenceSearch(const BitBoardTuple &tuple, T a = T::Min(),
                     T b = T::Max()) {
    auto qvalue = qsearch(tuple, a, b, 0);
    return qvalue;
  }

 private:
  T qsearch(const BitBoardTuple &tuple, T alpha, T beta, int depth) {
    m_stat.ExtraNode();

    CheckStopFlag();

    m_stat.EndNode();
    auto stand_pat = T::Value(tuple.board, m_color);
    stand_pat = tuple.board.CurrentColor() == m_color ? stand_pat : -stand_pat;

    if (stand_pat >= beta) return stand_pat;

    if (stand_pat < alpha - kBIG_DELTA) return alpha;

    if (alpha < stand_pat) alpha = stand_pat;

    BitBoardTuple::GenerateTuplesFast(
        Get(depth), tuple, tuple.board.CurrentColor(), kall,
        tuple.board.GetColorBitBoard(tuple.board.OpponentColor()), false);
    auto &moves = Get(depth);

    if (moves.empty()) return stand_pat;

    QuiescenceSearchOrdering(tuple.board, moves);
    T bestscore = T::Min();
    for (auto &sub : moves) {
      auto score = -qsearch(sub, -beta, -alpha, depth + 1);
      if (score >= beta) {  // beta cutoff
        bestscore = score;
        break;
      }
      if (score > bestscore) {
        bestscore = score;
        if (score > alpha) {
          alpha = score;
        };
      }
    }

    return bestscore;
  }
};
#endif
