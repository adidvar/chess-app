#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include <atomic>
#include <iostream>

#include <chesscore/bitboard.hpp>

#include "hktable.hpp"
#include "ordering.hpp"
#include "qsearch.hpp"
#include "search.hpp"
#include "ttable.hpp"

class AlphaBeta : public QSearch {
 public:
  AlphaBeta(const BitBoard &board, Color color) : QSearch(board, color) {}

  Score GetValue(int depth, Score a = Score::Min(), Score b = Score::Max()) {
    try {
      m_last_depth = depth;
      BitBoardTuple tuple{GetBoard(), GetBoard().Hash(), Turn()};
      return alphabeta(tuple, a, b, depth, depth);
    } catch (...) {
      m_last_turn = Turn();
      throw;
    }
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

  HKTable &GetBfTable() { return m_btable; }

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
          // m_last_turn = hashed->pv;
          // return hashed->value;
        }
      }
    }
    bool inCheck = tuple.board.Checkmate();
    /*

    if (GetSearchSettings().NullMoveEnabled() && depthleft >= 3 &&
        inCheck == false) {
      auto copy = tuple;
      copy.board.SkipMove();
      auto score = -alphabeta(copy, -beta, -beta + Score{1}, depthleft - 1 - 2,
                              depthmax);
      if (score >= beta) return beta;
    }
*/

    auto moves = tuple.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      m_last_turn = Turn();
      if (inCheck) return Score::CheckMate(depthleft, depthmax);
      return Score::Tie();
    }

    MainNode();

    ReOrder(tuple.board, moves, alpha, beta, m_btable, m_ttable, depthleft,
            depthmax, founded ? hashed->pv : Turn());

    Score bestscore = Score::Min();
    Turn bestturn = Turn();
    for (auto &sub : moves) {
      auto score = -alphabeta(sub, -beta, -alpha, depthleft - 1, depthmax);

      if (score >= beta) {  // beta cutoff
        bestscore = score;
        bestturn = sub.turn;
        m_btable.Increment(sub.turn, depthmax - depthleft);
        break;
      }
      if (score > bestscore) {
        bestscore = score;
        bestturn = sub.turn;
        if (score > alpha) {
          alpha = score;
          m_btable.Increment(sub.turn, depthmax - depthleft);
        };
      }
    }

    if (m_ttable != nullptr)
      m_ttable->Write(tuple.hash, oldalpha, beta, bestscore, bestturn,
                      depthleft);

    m_last_turn = bestturn;
    return bestscore;
  }

  TTable *m_ttable = nullptr;
  HKTable m_btable;

 protected:
  Turn m_last_turn{};
  int m_last_depth = 0;
};
#endif
