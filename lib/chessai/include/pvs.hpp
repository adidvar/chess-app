#ifndef PVS_HPP
#define PVS_HPP

#include <atomic>

#include "bitboard.hpp"
#include "ordering.hpp"
#include "qsearch.hpp"
#include "search.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

// #define DISTRIBUTION

class PVS : public QSearch {
  using T = Score;

 public:
  PVS(const BitBoard &board, Color color) : QSearch(board, color) {}

  T GetValue(int depth, T a = T::Min(), T b = T::Max()) {
    BitBoardTuple tuple{m_board, m_board.Hash(), Turn()};
    return pvs(tuple, a, b, depth, depth);
  }

  Turn GetTurn(int depth) {
    BitBoardTuple tuple{m_board, m_board.Hash(), Turn()};
    return pvturn(tuple, T::Min(), T::Max(), depth, depth);
  }

  std::vector<Turn> FindPV(int depth) { return findpv(m_board, depth); }

  Statistics GetStatistics() const { return m_stat; };
  BFTable &GetBfTable() { return m_btable; };

  TTable *GetTTable() const { return m_ttable; };
  void SetTTable(TTable *newTtable) { m_ttable = newTtable; };

 private:

  T pvs(const BitBoardTuple &tuple, T alpha, T beta, int depthleft,
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
        else if (hashed->type == TTableItem::FailHigh && hashed->value >= beta)
          return hashed->value;
        else if (hashed->type == TTableItem::FailHigh)
          alpha = std::max(alpha, hashed->value);
        //} else if (hashed->depth > depthleft) {
        //  if (hashed->type == TTableItem::PV) return hashed->value;
      }
    }

    auto moves = tuple.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      if (tuple.board.Checkmate()) return T::Lose(depthmax - depthleft);
      return T::Tie();
    }

    m_stat.MainNode();

    ReOrder(tuple.board, moves, alpha, beta, m_btable, m_ttable, depthleft,
            depthmax, founded ? hashed->pv : Turn());
    // BFTableReorderer(tuple.board, moves, m_btable, depthleft, depthmax,
    //                  founded ? hashed->pv : Turn());

    Turn bestturn = Turn();
    bool bSearchPv = true;
    for (auto &sub : moves) {
      T bestscore = T::Min();
      if (bSearchPv) {
        bestscore = -pvs(sub, -beta, -alpha, depthleft - 1, depthmax);
      } else {
        bestscore =
            -pvs(sub, -alpha - Score(1), -alpha, depthleft - 1, depthmax);
        if (bestscore > alpha)  // in fail-soft ... && score < beta ) is common
          bestscore = -pvs(sub, -beta, -alpha, depthleft - 1,
                           depthmax);  // re-search
      }
      if (bestscore >= beta) {
        bestturn = sub.turn;
        m_btable.Increment(sub.turn, depthleft, depthmax);
        return beta;  // fail-hard beta-cutoff
      }
      if (bestscore > alpha) {
        bestturn = sub.turn;
        m_btable.Increment(sub.turn, depthleft, depthmax);
        alpha = bestscore;  // alpha acts like max in MiniMax
        bSearchPv = false;  // *1)
      }
    }

    if (m_ttable != nullptr)
      m_ttable->Write(tuple.hash, oldalpha, beta, alpha, bestturn, depthleft,
                      depthmax);

    return alpha;
  }

  Turn pvturn(const BitBoardTuple &tuple, T alpha, T beta, int depthleft,
              int depthmax) {
    if (depthleft == 0) return Turn();

    bool founded = false;
    if (m_ttable) {
      const TTableItem *hashed = m_ttable->Search(tuple.hash, founded);
      return hashed->pv;
    }
    pvs(tuple, alpha, beta, depthleft, depthmax);
    if (m_ttable) {
      const TTableItem *hashed = m_ttable->Search(tuple.hash, founded);
      return hashed->pv;
    }
    return Turn{};
  }

  std::vector<Turn> findpv(BitBoard board, int depth) {
    std::vector<Turn> turns_;

    for (int i = 0; i < depth; i++) {
      auto turn = pvturn({board}, T::Min(), T::Max(), depth - i, depth);
      if (turn == Turn()) break;
      turns_.push_back(turn);
      board.ExecuteTurn(turn);
    }

    return turns_;
  }

  TTable *m_ttable = nullptr;
  BFTable m_btable;
};
#endif
