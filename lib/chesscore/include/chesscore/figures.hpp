#pragma once

#include <cassert>
#include <cinttypes>

class Figure {
 public:
     //empty
     static constexpr int8_t Empty = 0;
     //base figures
     static constexpr int8_t Pawn = 1;
     static constexpr int8_t Knight = 2;
     static constexpr int8_t Bishop = 3;
     static constexpr int8_t Rook = 4;
     static constexpr int8_t Queen = 5;
     static constexpr int8_t King = 6;
     //white figures
     static constexpr int8_t WPawn = 1;
     static constexpr int8_t WKnight = 2;
     static constexpr int8_t WBishop = 3;
     static constexpr int8_t WRook = 4;
     static constexpr int8_t WQueen = 5;
     static constexpr int8_t WKing = 6;
     //black figures
     static constexpr int8_t BPawn = -1;
     static constexpr int8_t BKnight = -2;
     static constexpr int8_t BBishop = -3;
     static constexpr int8_t BRook = -4;
     static constexpr int8_t BQueen = -5;
     static constexpr int8_t BKing = -6;

     constexpr Figure(int8_t value) noexcept
         : m_value(value)
     {}

     constexpr operator int8_t() const noexcept { return m_value; }

     constexpr bool isValid() const noexcept { return m_value >= BKing && m_value <= WKing; }
     constexpr bool isEmpty() const noexcept { return m_value == 0; }
     constexpr bool isWhite() const noexcept { return m_value > 0; }
     constexpr bool isBlack() const noexcept { return m_value < 0; }

     constexpr int8_t toIndex() const noexcept { return m_value < 0 ? -m_value : m_value; }

 private:
     int8_t m_value;
};

class Color {
public:
    constexpr Color(int8_t value) noexcept
        : m_value(value)
    {}
    static constexpr int8_t White{1};
    static constexpr int8_t Black{-1};
    constexpr operator int8_t() const noexcept { return m_value; }

private:
    int8_t m_value;
};
