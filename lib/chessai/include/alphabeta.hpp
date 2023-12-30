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

#define DISTRIBUTION

class AlphaBeta : public QSearch {
  using T = Score;

  // todo I rework to class to when search finds all we nee in one start or
  // throw exception and keep state

 public:
  AlphaBeta(const BitBoard &board, Color color) : QSearch(board, color) {}

  T GetValue(int depth, T a = T::Min(), T b = T::Max()) {
    BitBoardTuple tuple{GetBoard(), GetBoard().Hash(), Turn()};
    return alphabeta(tuple, a, b, depth, depth);
  }

  Turn GetTurn() {
    auto elem = m_ttable->GetLastElement();
    return elem.pv;
  }

  std::vector<Turn> FindPV() {
    auto elem = m_ttable->GetLastElement();
    auto depth = elem.depth;
    BitBoard board = GetBoard();

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

    return pv;
  }

  TTable *GetTTable() const { return m_ttable; }
  void SetTTable(TTable *newTtable) { m_ttable = newTtable; }

  BFTable &GetBfTable() { return m_btable; }

 private:
  T alphabeta(const BitBoardTuple &tuple, T alpha, T beta, int depthleft,
              int depthmax) {
    auto oldalpha = alpha;

    CheckStopFlag();

    if (depthleft == 0) {
#ifdef DISTRIBUTION
      auto value = QuiescenceSearch(tuple.board, alpha, beta);
      return value;
#else

      auto value = T::Value(tuple.board, m_color);
      return tuple.board.CurrentColor() == m_color ? value : -value;
#endif
    }

    bool founded = false;
    const TTableItem *hashed = nullptr;
    if (m_ttable) hashed = m_ttable->Search(tuple.hash, founded);

    if (founded) {
      if (hashed->depth == depthleft) {
        if (hashed->type == TTableItem::PV)
          return hashed->value;
        else if (hashed->type == TTableItem::FailHigh &&
                 hashed->value >= beta)
          return hashed->value;
        else if (hashed->type == TTableItem::FailHigh)
          alpha = std::max(alpha, hashed->value);
      } else if (hashed->depth > depthleft) {
        if (hashed->type == TTableItem::PV) return hashed->value;
      }
    }

    auto moves = tuple.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      if (tuple.board.Checkmate()) return T::CheckMate(depthleft, depthmax);
      return T::Tie();
    }

    m_stat.MainNode();

    // ReOrder(tuple.board, moves, alpha, beta, m_btable, m_ttable, depthleft,
    //         depthmax, founded ? hashed->pv : Turn());
    BFTableReorderer(tuple.board, moves, m_btable, depthleft, depthmax,
                     founded ? hashed->pv : Turn());

    T bestscore = T::Min();
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

    return bestscore;
  }

  Statistics m_stat;
  TTable *m_ttable = nullptr;
  BFTable m_btable;
};

#endif
