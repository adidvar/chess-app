#ifndef HKTABLE_H
#define HKTABLE_H

#include <array>
#include <deque>
#include <vector>

#include <chesscore/turn.hpp>

/**
 * @brief The BFTable class contains History heuristics and killer moves
 * counters
 */
class HKTable {
 public:
  /**
   * @brief Increment increments counters
   * @param turn turn for killer moves
   * @param reversed_depth max_depth - depth, 0 mean it's a root node
   */
  void Increment(Turn turn, uint8_t reversed_depth);
  /**
   * @brief GetKillerMoveCount returns a counter for a killer move
   * @param turn a killer move
   * @param reversed_depth max_depth - depth, 0 mean it's a leaf node
   * @return counter
   */
  [[nodiscard]] size_t GetKillerMoveCount(Turn turn,
                                          uint8_t reversed_depth) const;
  /**
   * @brief GetHistoryCount return history heuristic for some turn
   * @param turn
   * @return counter
   */
  [[nodiscard]] size_t GetHistoryCount(Turn turn) const;
  /**
   * @brief Sets counter to zero
   */
  void Clear();

 private:
  struct Frame {
    std::array<std::array<size_t, 64>, 64> data{};
  };
  using Data = std::deque<Frame>;

  Data m_killer_table;
  Frame m_history_table;
};

#endif
