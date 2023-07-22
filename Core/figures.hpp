#ifndef FIGURES_H
#define FIGURES_H

#include <inttypes.h>

/**
 * @brief The Figure class describes a chess figure
 */
class Figure {
 public:
  static constexpr uint8_t kEmpty = 0;   ///< Empty figure
  static constexpr uint8_t kPawn = 1;    ///< Pawn
  static constexpr uint8_t kKnight = 2;  ///< Knight
  static constexpr uint8_t kBishop = 3;  ///< Bishop
  static constexpr uint8_t kRook = 4;    ///< Rook
  static constexpr uint8_t kQueen = 5;   ///< Queen
  static constexpr uint8_t kKing = 6;    ///< King

  Figure(uint8_t value) noexcept : value_(value) {}

  Figure() noexcept : value_(kEmpty) {}

  operator uint8_t() const noexcept { return value_; }

  bool Valid() const noexcept { return value_ >= kEmpty && value_ <= kKing; }

  constexpr static uint8_t Max() { return 7; };

 private:
  uint8_t value_;
};

/**
 * @brief The Color class describes a chess figure color
 */
class Color {
  uint8_t value_;

 public:
  static constexpr uint8_t kWhite = 0;  ///< White
  static constexpr uint8_t kBlack = 1;  ///< Black

  Color(uint8_t value) noexcept : value_(value) {}

  Color() noexcept : value_(kWhite) {}

  operator uint8_t() const noexcept { return value_; }

  constexpr static uint8_t Max() { return 2; };
};

#endif  // FIGURES_H
