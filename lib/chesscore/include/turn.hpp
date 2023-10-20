#ifndef TURN_H
#define TURN_H

#include <string>

#include "figures.hpp"
#include "position.hpp"

/**
 * @brief Визначає звичайний шахматний
 *
 * Ходи бувають одного з 2 типів
 * 1) Звичайних хід
 * 2) Хід пішкою з вибором фігури
 */
class Turn {
 public:
  /**
   * @brief Пустий хід
   */
  Turn();
  /**
   * @brief Звичайний хід
   * @param move задає початкову і кінцеву позицію
   */
  Turn(Position from, Position to);
  /**
   * @brief Кінцевий хід пішкою
   * @param pawn_move Початкова кінцева і тип фігури
   */
  Turn(Position from, Position to, Figure figure);
  /**
   * @brief Рокірування
   * @param direction напрямок рокіровки
   */

  bool operator==(const Turn& turn) const;
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
