#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include <atomic>

#include "bftable.hpp"
#include "bitboard.hpp"
#include "exitcondition.hpp"
#include "ordering.hpp"
#include "qsearch.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

class AlphaBeta {
  using T = Evaluate;

  // todo I rework to class to when search finds all we nee in one start or
  // throw exception and keep state

 public:
  AlphaBeta(Color color) : m_color(color), m_search(color) {}

  T GetValue(const BitBoard &board, int depth, T a = T::Min(), T b = T::Max()) {
    clear();
    BitBoardTuple tuple{board, board.Hash(), Turn()};
    return alphabeta(tuple, a, b, depth, depth);
  }

  Turn GetTurn(const BitBoard &board, int depth) {
    clear();
    BitBoardTuple tuple{board, board.Hash(), Turn()};
    return alphabetaturn(tuple, T::Min(), T::Max(), depth, depth);
  }

  std::vector<Turn> FindPV(BitBoard board, int depth) {
    clear();
    return findpv(board, depth);
  }

  Statistics GetStatistics() const { return m_stat; };

  TTable *GetTTable() const { return m_ttable; };
  void SetTTable(TTable *newTtable) { m_ttable = newTtable; };

  std::atomic_bool *GetStopFlag() const;
  void SetStopFlag(std::atomic_bool *Stop_flag);

 private:
  void clear() {
    // m_btable.Clear();
    m_stat.Clear();
  };

  T alphabeta(const BitBoardTuple &tuple, T alpha, T beta, int depthleft,
              int depthmax) {
    auto oldalpha = alpha;

    if (m_stop_flag != nullptr) CheckAndThrow(*m_stop_flag);

    if (depthleft == 0) {
#ifdef DISTRIBUTION
      auto value = m_search.GetValue(tuple.board, alpha, beta);
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
        else if (hashed->type == TTableItem::FailHigh &&
                 hashed->value >= beta)
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
    T bestscore = T::Min();
    Turn bestturn = Turn();
    for (auto &sub : moves) {
      auto score = -alphabeta(sub, -beta, -alpha, depthleft - 1, depthmax);
      if (score >= beta) {  // beta cutoff
        bestscore = score;
        bestturn = sub.turn;
        m_btable.Push(sub.turn, depthleft);
        break;
      }
      if (score > bestscore) {
        bestscore = score;
        bestturn = sub.turn;
        if (score > alpha) {
          alpha = score;
          m_btable.Push(sub.turn, depthleft);
        };
      }
    }

    if (hashed != nullptr && (((founded && hashed->depth <= depthleft)))) {
      auto hv = bestscore;
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
      hashed->value = bestscore;
      hashed->pv = bestturn;
      hashed->depth = depthleft;
    }

    return bestscore;
  }

  Turn alphabetaturn(const BitBoardTuple &tuple, T alpha, T beta, int depthleft,
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
        else if (hashed->type == TTableItem::FailHigh &&
                 hashed->value >= beta)
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
      auto score = -alphabeta(sub, -beta, -alpha, depthleft - 1, depthmax);
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
      auto turn = alphabetaturn({board}, T::Min(), T::Max(), depth - i, depth);
      if (turn == Turn()) break;
      turns_.push_back(turn);
      board.ExecuteTurn(turn);
    }

    return turns_;
  }

  std::atomic_bool *m_stop_flag = nullptr;
  Color m_color;
  Statistics m_stat;
  BFTable m_btable;
  TTable *m_ttable = nullptr;
  QSearch m_search;
};

inline std::atomic_bool *AlphaBeta::GetStopFlag() const { return m_stop_flag; }

inline void AlphaBeta::SetStopFlag(std::atomic_bool *Stop_flag) {
  m_stop_flag = Stop_flag;
}

#endif
