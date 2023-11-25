#ifndef PVS_HPP
#define PVS_HPP

#include <atomic>

#include "bitboard.hpp"
#include "ordering.hpp"
#include "qsearch.hpp"
#include "search.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

class PVS : public QSearch {
  using T = Score;

 public:
  PVS(Color color) : QSearch(color) {}

  T GetValue(const BitBoard &board, int depth, T a = T::Min(), T b = T::Max()) {
    clear();
    BitBoardTuple tuple{board, board.Hash(), Turn()};
    return pvs(tuple, a, b, depth, depth);
  }

  Turn GetTurn(const BitBoard &board, int depth) {
    clear();
    BitBoardTuple tuple{board, board.Hash(), Turn()};
    return pvturn(tuple, T::Min(), T::Max(), depth, depth);
  }

  std::vector<Turn> FindPV(BitBoard board, int depth) {
    clear();
    return findpv(board, depth);
  }

  Statistics GetStatistics() const { return m_stat; };

  TTable *GetTTable() const { return m_ttable; };
  void SetTTable(TTable *newTtable) { m_ttable = newTtable; };

 private:
  void clear() {
    // m_btable.Clear();
    m_stat.Clear();
  };

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
    TTableItem *hashed = nullptr;
    if (m_ttable) hashed = m_ttable->Search(tuple.hash, founded);

    if (founded) {
      if (hashed->depth == depthleft) {
        if (hashed->type == TTableItem::PV)
          return hashed->value;
        else if (hashed->type == TTableItem::FailHigh && hashed->value >= beta)
          return hashed->value;
        else if (hashed->type == TTableItem::FailHigh)
          alpha = std::max(alpha, hashed->value);
      } else if (hashed->depth > depthleft) {
#ifdef DISTRIBUTION
        if (hashed->type == TTableItem::PV) return hashed->value;
#endif
      }
    }

    auto moves =
        tuple.board.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      if (tuple.board.Checkmate()) return T::Lose(depthmax - depthleft);
      return T::Tie();
    }

    m_stat.MainNode();

    ReOrder(tuple.board, moves, alpha, beta, m_btable, m_ttable, depthleft,
            depthmax, founded ? hashed->pv : Turn());
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
        m_btable.Push(sub.turn, depthleft);
        return beta;  // fail-hard beta-cutoff
      }
      if (bestscore > alpha) {
        bestturn = sub.turn;
        m_btable.Push(sub.turn, depthleft);
        alpha = bestscore;  // alpha acts like max in MiniMax
        bSearchPv = false;  // *1)
      }
    }

    if (hashed != nullptr && (((founded && hashed->depth <= depthleft)))) {
      auto hv = alpha;
      auto ha = oldalpha;
      auto hb = beta;

      if (hv <= ha)
        hashed->type = TTableItem::FailLow;
      else if (hv >= hb)
        hashed->type = TTableItem::FailHigh;
      else
        hashed->type = TTableItem::PV;

      hashed->hasvalue = true;
      hashed->hash = tuple.hash;
      hashed->value = alpha;
      hashed->pv = bestturn;
      hashed->depth = depthleft;
    }

    return alpha;
  }

  Turn pvturn(const BitBoardTuple &tuple, T alpha, T beta, int depthleft,
              int depthmax) {
    m_stat.MainNode();

    if (depthleft == 0) return Turn();

    bool founded = false;
    TTableItem *hashed = nullptr;
    if (m_ttable) hashed = m_ttable->Search(tuple.hash, founded);

    if (founded) {
      if (hashed->depth == depthleft) {
        if (hashed->type == TTableItem::PV)
          return hashed->pv;
        else if (hashed->type == TTableItem::FailHigh && hashed->value >= beta)
          return Turn();
        else if (hashed->type == TTableItem::FailHigh)
          alpha = std::max(alpha, hashed->value);
      } else if (hashed->depth > depthleft) {
#ifdef DISTRIBUTION
        if (hashed->type == TTableItem::PV) return hashed->pv;
#endif
      }
    }

    auto moves =
        BitBoard::GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      return Turn();
    }

    ReOrder(tuple.board, moves, alpha, beta, m_btable, m_ttable, depthleft,
            depthmax, founded ? hashed->pv : Turn());
    T bestscore = T::Min();
    Turn turn = Turn();
    for (auto &sub : moves) {
      auto score = -pvs(sub, -beta, -alpha, depthleft - 1, depthmax);
      if (score >= beta) return Turn();
      if (score > bestscore) {
        bestscore = score;
        turn = sub.turn;
        if (score > alpha) alpha = score;
      }
    }
    return turn;
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
