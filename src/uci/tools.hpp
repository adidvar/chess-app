#pragma once

#include <chesscore/bitboard.hpp>
#include <chrono>
#include <cmath>
#include <iostream>

inline size_t countPerft(const BitBoard &board, size_t depth) {
  if (depth == 0) return 1;
  size_t counter = 0;
  bool is_mate;
  Turn turns[216];
  int count = board.getTurns(board.getCurrentSide(), turns, is_mate);
  if (depth == 1) return count;
  for (int i = 0; i < count; i++) {
    counter += countPerft(board.executeTurn(board.getCurrentSide(), turns[i]),
                          depth - 1);
  }
  return counter;
}

inline void perft(const BitBoard &board, int depth) {
  if (depth == 0) return;

  using std::chrono::high_resolution_clock;
  using namespace std::chrono;

  using custom_duration = duration<float, std::micro>;

  auto begin = high_resolution_clock::now();

  size_t counter = 0;
  bool is_mate;
  Turn turns[216];
  int count = board.getTurns(board.getCurrentSide(), turns, is_mate);
  for (int i = 0; i < count; i++) {
    int subcounter = countPerft(
        board.executeTurn(board.getCurrentSide(), turns[i]), depth - 1);
    std::cout << turns[i].toString() << ": " << subcounter << std::endl;
    counter += subcounter;
  }

  auto end = high_resolution_clock::now();

  std::cout << std::endl;
  std::cout << "Nodes searched: " << counter << std::endl;
  std::cout << "NPS: "
            << (long long unsigned)std::floor(
                   counter /
                   duration_cast<custom_duration>(end - begin).count())
            << "M" << std::endl;
}
