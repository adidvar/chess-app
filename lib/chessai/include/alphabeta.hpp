#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include <atomic>
#include <iostream>

#include "bftable.hpp"
#include "bitboard.hpp"
#include "ordering.hpp"
#include "qsearch.hpp"
#include "search.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

class AlphaBeta : public QSearch {
 public:
  AlphaBeta(const BitBoard &board, Color color) : QSearch(board, color) {}

  Score GetValue(int depth, Score a = Score::Min(), Score b = Score::Max()) {
    m_last_depth = depth;
    BitBoardTuple tuple{GetBoard(), GetBoard().Hash(), Turn()};
    return alphabeta(tuple, a, b, depth, depth);
  }

  Turn GetTurn() { return m_last_turn; }

  std::vector<Turn> FindPV() {
    auto last_turn = m_last_turn;
    BitBoard board = GetBoard();
    int depth = m_last_depth;

    std::vector<Turn> pv;
    int i = 0;
    do {
      BitBoardTuple tuple(board, board.Hash(), Turn());
      alphabeta(tuple, Score::Min(), Score::Max(), depth - i, depth);
      auto turn = GetTurn();
      if (!turn.Valid()) break;
      pv.push_back(turn);
      board.ExecuteTurn(turn);
      i++;
    } while (pv.back().Valid() && depth != i);

    // restore state
    m_last_turn = last_turn;
    return pv;
  }

  TTable *GetTTable() const { return m_ttable; }
  void SetTTable(TTable *newTtable) { m_ttable = newTtable; }

  BFTable &GetBfTable() { return m_btable; }

 private:
  Score alphabeta(const BitBoardTuple &tuple, Score alpha, Score beta,
                  int depthleft, int depthmax) {
    auto oldalpha = alpha;

    CheckStopFlag();

    if (depthleft == 0) {
      auto value = QuiescenceSearch(tuple.board, alpha, beta);
      return value;

      /*
      auto value = T::Value(tuple.board, m_color);
      return tuple.board.CurrentColor() == m_color ? value : -value;
*/
    }

    bool founded = false;
    const TTableItem *hashed = nullptr;
    if (m_ttable) hashed = m_ttable->Search(tuple.hash, founded);

    if (founded) {
      if (hashed->depth == depthleft) {
        if (hashed->type == TTableItem::PV) {
          m_last_turn = hashed->pv;
          return hashed->value;
        } else if (hashed->type == TTableItem::FailHigh &&
                   hashed->value >= beta) {
          m_last_turn = hashed->pv;
          return hashed->value;
        } else if (hashed->type == TTableItem::FailHigh)
          alpha = std::max(alpha, hashed->value);
      } else if (hashed->depth > depthleft) {
        if (hashed->type == TTableItem::PV) {
          m_last_turn = hashed->pv;
          return hashed->value;
        }
      }
    }
    bool inCheck = tuple.board.Checkmate();

    if (depthleft >= 3 && inCheck == false) {
      auto copy = tuple;
      copy.board.SkipMove();
      auto score = -alphabeta(copy, -beta, -beta + Score{1}, depthleft - 1 - 2,
                              depthmax);
      if (score >= beta) return beta;
    }

    /*
    if (depthleft == 1) {
      const static Score kBIG_DELTA{Score::GetFigureScore(Figure::kBishop)};
      auto stand_pat =
          Score::GetStaticValue(tuple.board, tuple.board.CurrentColor(),
                                GetSearchSettings().GetStage());
      if (stand_pat < alpha - kBIG_DELTA) return alpha;
    }
    if (depthleft == 2) {
      const static Score kBIG_DELTA{Score::GetFigureScore(Figure::kQueen)};
      auto stand_pat =
          Score::GetStaticValue(tuple.board, tuple.board.CurrentColor(),
                                GetSearchSettings().GetStage());
      if (stand_pat < alpha - kBIG_DELTA) return alpha;
    }
*/

    auto moves = tuple.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      m_last_turn = Turn();
      if (inCheck) return T::CheckMate(depthleft, depthmax);
      return T::Tie();
    }

    GetStatistics().MainNode();

    ReOrder(tuple.board, moves, alpha, beta, m_btable, m_ttable, depthleft,
            depthmax, founded ? hashed->pv : Turn());
    // BFTableReorderer(tuple.board, moves, m_btable, depthleft, depthmax,
    //                  founded ? hashed->pv : Turn());

    Score bestscore = Score::Min();
    Turn bestturn = Turn();
    for (auto &sub : moves) {
      auto score = -alphabeta(sub, -beta, -alpha, depthleft - 1, depthmax);

      if (score >= beta) {  // beta cutoff
        bestscore = score;
        bestturn = sub.turn;
        m_btable.Increment(sub.turn, depthleft, depthmax);
        break;
      }
      if (score > bestscore) {
        bestscore = score;
        bestturn = sub.turn;
        if (score > alpha) {
          alpha = score;
          m_btable.Increment(sub.turn, depthleft, depthmax);
        };
      }
    }

    if (m_ttable != nullptr)
      m_ttable->Write(tuple.hash, oldalpha, beta, bestscore, bestturn,
                      depthleft, depthmax);

    m_last_turn = bestturn;
    return bestscore;
  }

  TTable *m_ttable = nullptr;
  BFTable m_btable;

  Turn m_last_turn;
  int m_last_depth;
};
#endif
