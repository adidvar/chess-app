#ifndef QSEARCH_HPP
#define QSEARCH_HPP

#include <algorithm>

#include "bitboard.hpp"
#include "bitboardtuple.hpp"
#include "score.hpp"
#include "search.hpp"
#include "statistics.hpp"

class ChessTreeHash {
 public:
  std::vector<BitBoardTuple> &Get(int depth) {
    m_turns.resize(depth + 1);
    return m_turns[depth];
  }

 private:
  std::vector<std::vector<BitBoardTuple>> m_turns;
};

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
        Score::GetFigureScore(to_figure) - Score::GetFigureScore(from_figure);
    elem.priority.type =
        (int)delta_price >= 0 ? PositiveAttack : NegativeAttack;
    elem.priority.index = delta_price;
  }
  std::sort(vector.rbegin(), vector.rend());
}

class QSearch : public Search, private ChessTreeHash {
  using T = Score;

  inline const static Score kBIG_DELTA{
      Score::GetFigureScore(Figure::kPawn) +
      Score::GetFigureScore(Figure::kQueen)};  // queen value

 public:
  QSearch(const BitBoard &board, Color color) : Search(board, color) {}

  T QuiescenceSearch(const BitBoardTuple &tuple, T a = T::Min(),
                     T b = T::Max()) {
    auto qvalue = qsearch(tuple, a, b, 0);
    return qvalue;
  }

 private:
  T qsearch(const BitBoardTuple &tuple, T alpha, T beta, int depth) {
    GetStatistics().ExtraNode();

    CheckStopFlag();

    GetStatistics().EndNode();
    T stand_pat = T::GetStaticValue(tuple.board, tuple.board.CurrentColor(),
                                    GetSearchSettings().GetStage());

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
