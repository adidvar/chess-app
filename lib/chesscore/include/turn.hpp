#ifndef TURN_H
#define TURN_H

#include <string>

#include "figures.hpp"
#include "position.hpp"

/**
 * @brief Describes a chess turn class
 */
class Turn {
 public:
  /**
   * @brief Invalid turn
   */
  Turn();
  /**
   * @brief Default turn
   */
  Turn(Position from, Position to);
  Turn(Position from, Position to, Figure figure);

  bool operator==(const Turn& turn) const;
  bool operator!=(const Turn& turn) const;
  bool operator<(const Turn& turn) const;

  [[nodiscard]] Position from() const noexcept;
  [[nodiscard]] Position to() const noexcept;
  [[nodiscard]] Position& from() noexcept;
  [[nodiscard]] Position& to() noexcept;
  [[nodiscard]] Figure figure() const noexcept;
  [[nodiscard]] Figure& figure() noexcept;

  [[nodiscard]] bool Valid() const noexcept;

  [[nodiscard]] bool IsCastling() const noexcept;
  [[nodiscard]] bool IsLongCastling() const noexcept;
  [[nodiscard]] bool IsShortCastling() const noexcept;

  [[nodiscard]] bool IsTrasformation() const noexcept;

  [[nodiscard]] std::string ToChessFormat() const;

  static Turn FromChessFormat(std::string_view string);
  static Turn GetShortCastling(Color color);
  static Turn GetLongCastling(Color color);

 private:
  Position from_;
  Position to_;
  Figure figure_;
};

#endif  // TURN_H
