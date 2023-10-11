#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include <atomic>

#include "bftable.hpp"
#include "bitboard.hpp"
#include "exitcondition.hpp"
#include "ordering.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

class AlphaBeta {
  using T = Evaluate;

  // todo I rework to class to when search finds all we nee in one start or
  // throw exception and keep state

 public:
  AlphaBeta(Color color) : m_color(color) {}

  T GetValue(const BitBoard &board, int depth, T a = T::Min(), T b = T::Max()) {
    clear();
    BitBoardTuple tuple{board, board.Hash(), Turn()};
    return alphabeta(tuple, a, b, depth, depth);
  }

  Turn GetTurn(const BitBoard &board, int depth) {
    clear();
    BitBoardTuple tuple{board, board.Hash(), Turn()};
    return alphabetaturn(tuple, T::Min(), T::Max(), depth, depth).second;
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
      // auto value = qsearch_.GetValue(tuple.board, alpha, beta);
      auto value = T::Value(tuple.board, m_color);
      return tuple.board.CurrentColor() == m_color ? value : -value;
      // return value;
    }

    bool founded = false;
    SearchElement *hashed;
    if (m_ttable) hashed = m_ttable->Search(tuple.hash, founded);

    if (founded) {
      if (hashed->depth == depthleft && hashed->type == SearchElement::PV)
        return hashed->value;
      else if (hashed->depth == depthleft &&
               hashed->type == SearchElement::FailHigh && hashed->value > beta)
        return hashed->value;
      else if (hashed->depth == depthleft &&
               hashed->type == SearchElement::FailHigh)
        alpha = std::max(alpha, hashed->value);
      else if (hashed->depth == depthleft &&
               hashed->type == SearchElement::FailLow)
        beta = std::min(beta, hashed->value);
      // else if (hashed->depth >= depthleft && hashed->type ==
      // SearchElement::PV)
      //   return hashed->value;
    }

    m_stat.MainNode();

    auto moves =
        tuple.board.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      if (tuple.board.Checkmate()) return T::Lose(depthmax - depthleft);
      return T::Tie();
    }

    ReOrder(tuple.board, moves, m_btable, m_ttable);
    T bestscore = T::Min();
    for (auto &sub : moves) {
      auto score = -alphabeta(sub, -beta, -alpha, depthleft - 1, depthmax);
      if (score >= beta) {  // beta cutoff
        bestscore = score;
        m_btable.Push(sub.turn);
        break;
      }
      if (score > bestscore) {
        bestscore = score;
        if (score > alpha) {
          alpha = score;
          m_btable.Push(sub.turn);
        };
      }
    }

    if (m_ttable) {
      hashed->hasvalue = true;
      hashed->hash = tuple.hash;
      hashed->value = bestscore;
      hashed->depth = depthleft;

      if (bestscore > oldalpha && bestscore < beta)
        hashed->type = SearchElement::PV;
      else if (bestscore <= oldalpha)
        hashed->type = SearchElement::FailLow;
      else if (bestscore >= beta)
        hashed->type = SearchElement::FailHigh;
    }

    return bestscore;
  }

  std::pair<T, Turn> alphabetaturn(const BitBoardTuple &tuple, T alpha, T beta,
                                   int depthleft, int depthmax) {
    m_stat.MainNode();

    auto moves =
        tuple.board.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      return {T(), Turn()};
    }

    ReOrder(tuple.board, moves, m_btable, m_ttable);
    T bestscore = T::Min();
    Turn turn = Turn();
    for (auto &sub : moves) {
      auto score = -alphabeta(sub, -beta, -alpha, depthleft - 1, depthmax);
      if (score >= beta) return {score, Turn()};
      if (score > bestscore) {
        bestscore = score;
        turn = sub.turn;
        if (score > alpha) {
          alpha = score;
          m_btable.Push(sub.turn);
        };
      }
    }
    return {bestscore, turn};
  }

  std::vector<Turn> findpv(BitBoard board, int depth) {
    std::vector<Turn> turns_;

    for (size_t i = 0; i < depth; i++) {
      auto turn = alphabetaturn({board, board.Hash(), Turn()}, T::Min(),
                                T::Max(), depth - i, depth);
      if (turn.second == Turn()) break;
      turns_.push_back(turn.second);
      board.ExecuteTurn(turn.second);
    }

    return turns_;
  }

  std::atomic_bool *m_stop_flag = nullptr;
  Color m_color;
  Statistics m_stat;
  BFTable m_btable;
  TTable *m_ttable = nullptr;
};

inline std::atomic_bool *AlphaBeta::GetStopFlag() const { return m_stop_flag; }

inline void AlphaBeta::SetStopFlag(std::atomic_bool *Stop_flag) {
  m_stop_flag = Stop_flag;
}

#endif
