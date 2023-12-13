#ifndef MINIMAX_HPP
#define MINIMAX_HPP

#include "bitboard.hpp"
#include "bitboardtuple.hpp"
#include "evaluate.hpp"
#include "search.hpp"
#include "statistics.hpp"

class MinMax : public Search {
  using T = Score;

  Statistics m_stat;

  T minimax(const BitBoard &bitboard, size_t depth, size_t max_depth) {
    if (depth == 0) {
      auto approx = T::Value(bitboard, m_color);
      return bitboard.CurrentColor() == m_color ? approx : -approx;
    }

    CheckStopFlag();

    auto nodes = bitboard.GenerateSubBoards(bitboard.CurrentColor());

    if (nodes.empty()) {
      if (bitboard.Checkmate()) return T::Lose(max_depth - depth);
      return T::Tie();
    }

    T bestscore = T::Min();
    for (auto &sub : nodes) {
      auto score = -minimax(sub, depth - 1, max_depth);
      bestscore = std::max(score, bestscore);
    }
    return bestscore;
  }
  std::vector<Turn> findpv(BitBoard board, int depth) {
    std::vector<Turn> turns_;

    for (size_t i = 0; i < depth; i++) {
      auto turn = minimaxturn({board}, depth - i, depth);
      if (turn.second == Turn()) break;
      turns_.push_back(turn.second);
      board.ExecuteTurn(turn.second);
    }

    return turns_;
  }

  std::pair<T, Turn> minimaxturn(const BitBoardTuple &tuple, int depthleft,
                                 int depthmax) {
    m_stat.MainNode();

    auto moves =
        BitBoardTuple::GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      return {T(), Turn()};
    }

    T bestscore = T::Min();
    Turn turn = Turn();
    for (auto &sub : moves) {
      auto score = -minimax(sub.board, depthleft - 1, depthmax);
      if (score > bestscore) {
        bestscore = score;
        turn = sub.turn;
      }
    }
    return {bestscore, turn};
  }

 public:
  explicit MinMax(Color color) : Search(color) {}

  T GetValue(const BitBoard &board, int depth) {
    m_stat.Clear();
    return minimax(board, depth, depth);
  }

  Turn GetTurn(const BitBoard &board, int depth) {
    m_stat.Clear();
    return minimaxturn({board}, depth, depth).second;
  }

  std::vector<Turn> FindPV(BitBoard board, int depth) {
    m_stat.Clear();
    return findpv(board, depth);
  }

  Statistics GetStatistics() const { return m_stat; };
};

#endif
