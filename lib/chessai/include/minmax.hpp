#ifndef MINIMAX_HPP
#define MINIMAX_HPP

#include <atomic>
#include <bitboard.hpp>
#include <evaluate.hpp>
#include <exitcondition.hpp>
#include <statistics.hpp>

class MinMax {
  using T = Evaluate;

  Color m_color;
  Statistics m_stat;
  std::atomic_bool *m_stop_flag = nullptr;

  T minimax(const BitBoard &bitboard, size_t depth, size_t max_depth) {
    if (depth == 0) {
      auto approx = T::Value(bitboard, m_color);
      return bitboard.CurrentColor() == m_color ? approx : -approx;
    }

    if (m_stop_flag != nullptr) CheckAndThrow(*m_stop_flag);

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
      auto turn =
          minimaxturn({board, board.Hash(), Turn(), 0}, depth - i, depth);
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
        BitBoard::GenerateTuplesFast(tuple, tuple.board.CurrentColor());

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
  explicit MinMax(Color color) : m_color(color) {}

  T GetValue(const BitBoard &board, int depth) {
    m_stat.Clear();
    return minimax(board, depth, depth);
  }

  Turn GetTurn(const BitBoard &board, int depth) {
    m_stat.Clear();
    return minimaxturn({board, board.Hash(), Turn(), 0}, depth, depth).second;
  }

  std::vector<Turn> FindPV(BitBoard board, int depth) {
    m_stat.Clear();
    return findpv(board, depth);
  }

  Statistics GetStatistics() const { return m_stat; };

  std::atomic_bool *GetStopFlag() const { return m_stop_flag; };
  void SetStopFlag(std::atomic_bool *Stop_flag) { m_stop_flag = Stop_flag; };
};

#endif
