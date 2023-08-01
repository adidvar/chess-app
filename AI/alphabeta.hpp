#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include <algorithm>
#include <iostream>

#include "bitboard.hpp"
#include "qsearch.hpp"
#include "statistics.hpp"
#include "ttable.hpp"

template <typename T>
class AlphaBeta {
 public:
  AlphaBeta(Color color, Statistics &stat)
      : qsearch_(color, stat), color_(color), stat_(stat) {}

  T GetValue(const BitBoard &board, int depth, T a = T::Min(), T b = T::Max()) {
    BitBoardTuple tuple{board, board.Hash(), Turn()};
    return alphabeta(tuple, a, b, depth, depth);
  }
  std::vector<Turn> FindPV(BitBoard board, int depth) {
    return findpv(board, depth);
  }

  static T Evaluate(BitBoard board, Color color, int depth, Statistics &stat) {
    AlphaBeta<T> core(color, stat);
    return core.GetValue(board, depth);
  }

 private:
  T alphabeta(const BitBoardTuple &tuple, T alpha, T beta, int depthleft,
              int depthmax) {
    if (depthleft == 0) {
      auto value = qsearch_.GetValue(tuple.board, alpha, beta);
      return tuple.board.CurrentColor() == color_ ? value : -value;
    }

    bool founded = false;
    auto hashed = table_.Search(tuple.hash, founded);

    if (founded && hashed->depth >= depthleft) {
      return hashed->value;
    }

    stat_.MainNode();

    auto moves =
        tuple.board.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      if (tuple.board.Checkmate()) return T::Lose(depthmax - depthleft);
      return T::Tie();
    }

    ReOrder(tuple.board, moves.rbegin(), moves.rend());
    T bestscore = T::Min();
    for (auto &sub : moves) {
      auto score = -alphabeta(sub, -beta, -alpha, depthleft - 1, depthmax);
      if (score >= beta) {
        bestscore = score;
        break;
      }
      if (score > bestscore) {
        bestscore = score;
        if (score > alpha) alpha = score;
      }
    }

    hashed->hasvalue = true;
    hashed->hash = tuple.hash;
    hashed->value = bestscore;
    hashed->depth = depthleft;
    if (bestscore <= alpha)
      hashed->type = SearchElement::FailLow;
    else if (bestscore >= beta)
      hashed->type = SearchElement::FailHigh;
    else
      hashed->type = SearchElement::PV;

    return bestscore;
  }

  std::pair<T, Turn> alphabetaturn(const BitBoardTuple &tuple, T alpha, T beta,
                                   int depthleft, int depthmax) {
    stat_.MainNode();

    auto moves =
        tuple.board.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      return {T(), Turn()};
    }

    ReOrder(tuple.board, moves.rbegin(), moves.rend());
    T bestscore = T::Min();
    Turn turn = Turn();
    for (auto &sub : moves) {
      auto score = -alphabeta(sub, -beta, -alpha, depthleft - 1, depthmax);
      if (score >= beta) return {score, Turn()};
      if (score > bestscore) {
        bestscore = score;
        turn = sub.turn;
        if (score > alpha) alpha = score;
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

  Color color_;

  Statistics &stat_;
  TTable table_;
  QSearch<T> qsearch_;
};

#endif
