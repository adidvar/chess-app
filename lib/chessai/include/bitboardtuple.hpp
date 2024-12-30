#ifndef BITBOARDTUPLE_HPP
#define BITBOARDTUPLE_HPP

#include <chesscore/bitboard.hpp>

#include "score.hpp"

struct BitBoardTuple {
  BitBoardTuple(const BitBoard &board)
      : board(board), hash(board.Hash()), turn{}, priority{} {}

  BitBoardTuple(const BitBoard &board, bitboard_hash_t hash, Turn turn)
      : board(board), hash(hash), turn{turn}, priority{} {}

  BitBoard board;
  bitboard_hash_t hash;
  Turn turn;

  Score value;
  int depth;

  struct Priority {
    int64_t type;
    int64_t index;
  } priority;

  bool operator<(const BitBoardTuple &t1) const noexcept {
    return std::tie(priority.type, priority.index) <
           std::tie(t1.priority.type, t1.priority.index);
  }
  /**
   * @brief GenerateTuplesFast generates subboards subturns and subhashes
   * @param tuple input tuple
   * @param color color
   * @param from from mask
   * @param to to mask
   * @return array of tuples
   * @attention function is much faster than split generation
   */
  static std::vector<BitBoardTuple> GenerateTuplesFast(BitBoardTuple tuple,
                                                       Color color,
                                                       uint64_t from = kall,
                                                       uint64_t to = kall) {
    auto boards = tuple.board.GenerateSubBoards(color, from, to);

    std::vector<BitBoardTuple> tuples;
    tuples.reserve(boards.size());

    for (auto &board : boards) {
      auto turn = BitBoard::GenerateTurn(tuple.board, board, color);
      bitboard_hash_t hash =
          BitBoard::GenerateHash(tuple.board, tuple.hash, turn, board);
      tuples.emplace_back(board, hash, turn);
    }

    return tuples;
  }

  static void GenerateTuplesFast(std::vector<BitBoardTuple> &list,
                                 BitBoardTuple tuple, Color color,
                                 uint64_t from = kall, uint64_t to = kall,
                                 bool genHash = true) {
    static thread_local std::vector<BitBoard> boards;

    boards = tuple.board.GenerateSubBoards(color, from, to);

    list.reserve(boards.size());
    list.clear();

    for (auto &board : boards) {
      auto turn = BitBoard::GenerateTurn(tuple.board, board, color);
      bitboard_hash_t hash = 0;
      if (genHash)
        hash = BitBoard::GenerateHash(tuple.board, tuple.hash, turn, board);
      list.emplace_back(board, hash, turn);
    }
  }
};

#endif
