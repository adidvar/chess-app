#pragma once

#include <cinttypes>
#include <string>

/*
+--+--+--+--+--+--+--+--+
| 0| 1| 2| 3| 4| 5| 6| 7|
+--+--+--+--+--+--+--+--+
| 8| 9|10|11|12|13|14|15|
+--+--+--+--+--+--+--+--+
|16|17|18|19|20|21|22|23|
+--+--+--+--+--+--+--+--+
|24|25|26|27|28|29|30|31|
+--+--+--+--+--+--+--+--+
|32|33|34|35|36|37|38|39|
+--+--+--+--+--+--+--+--+
|40|41|42|43|44|45|46|47|
+--+--+--+--+--+--+--+--+
|48|49|50|51|52|53|54|55|
+--+--+--+--+--+--+--+--+
|56|57|58|59|60|61|62|63|
+--+--+--+--+--+--+--+--+
*/

/**
 * @brief The Position class describes a chess position
 */
class Position
{
public:
    constexpr Position(uint8_t x, uint8_t y) noexcept { m_index = y * 8 + x; }

    constexpr Position() noexcept
        : m_index(k_invalid_position)
    {}

    constexpr Position(uint8_t index) noexcept
        : m_index(index)
    {}
    constexpr explicit Position(std::string_view string)
    {
        if (string.size() == 2 && string[0] >= 'a' && string[0] <= 'h' && string[1] >= '1'
            && string[1] <= '8')
            *this = Position(string[0] - 'a', '8' - string[1]);
        else
            *this = {};
    }

    [[nodiscard]] constexpr bool isValid() const noexcept { return m_index < k_invalid_position; }

    [[nodiscard]] constexpr uint8_t index() const noexcept { return m_index; }

    [[nodiscard]] constexpr uint8_t x() const noexcept { return m_index % 8; }
    [[nodiscard]] constexpr uint8_t y() const noexcept { return m_index / 8; }

    [[nodiscard]] constexpr Position rotate() const noexcept { return Position(63 - m_index); }

    [[nodiscard]] std::string toString() const
    {
        if (!isValid())
            return "--";
        std::string str = "00";
        str[0] = static_cast<char>('a' + x());
        str[1] = static_cast<char>('8' - y());
        return str;
    }

    bool operator==(const Position& pos) const noexcept { return m_index == pos.m_index; }
    bool operator!=(const Position& pos) const noexcept { return m_index != pos.m_index; }
    bool operator<(const Position& pos) const noexcept { return m_index < pos.m_index; }

private:
    static constexpr uint8_t k_invalid_position = 64;
    uint8_t m_index;
};

constexpr Position operator"" _p(const char* str, std::size_t len)
{
    return Position(std::string_view(str, len));
}

constexpr uint8_t operator"" _pv(const char* str, std::size_t len)
{
    return Position(std::string_view(str, len)).index();
}

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

class Turn
{
public:
    constexpr Turn();
    constexpr Turn(const Turn& other) = default;

    constexpr Turn(Position from, Position to, bool attack = false);
    constexpr Turn(Position from, Position to, Figure figure, bool attack = false);

    explicit Turn(std::string_view chess_format);
    [[nodiscard]] std::string toString() const;

    constexpr bool operator==(const Turn& other) const = default;
    constexpr bool operator!=(const Turn& other) const = default;

    constexpr Turn& operator=(const Turn& other) = default;

    [[nodiscard]] constexpr  Position from() const noexcept;
   [[nodiscard]] constexpr  Position to() const noexcept;
   [[nodiscard]] constexpr  Figure figure() const noexcept;

   [[nodiscard]] constexpr bool isValid() const noexcept;
    [[nodiscard]] constexpr  bool isTrasformation() const noexcept;
    [[nodiscard]] constexpr bool isAttack() const noexcept;

    constexpr void setFigure(Figure figure);

private:
    uint16_t m_from : 6;
    uint16_t m_to : 6;
    uint16_t m_attack : 1 = false;
    uint16_t m_figure : 3 = Figure::Empty;
};

static_assert(sizeof(Turn) == 2, "Turn must be exactly 2 bytes!");

constexpr Turn::Turn()
    : m_from(0)
    , m_to(0)
    , m_attack(false)
    , m_figure(7)
{}

constexpr Turn::Turn(Position from, Position to, bool attack)
    : m_from(from.index())
    , m_to(to.index())
    , m_attack(attack)
{}

constexpr Turn::Turn(Position from, Position to, Figure figure, bool attack)
    : m_from(from.index())
    , m_to(to.index())
    , m_figure(figure)
    , m_attack(attack)
{}

constexpr bool Turn::isValid() const noexcept
{
    return this->m_figure != 7;
}

constexpr Position Turn::from() const noexcept
{
    return m_from;
}

constexpr Position Turn::to() const noexcept
{
    return m_to;
}

constexpr bool Turn::isTrasformation() const noexcept
{
    return m_figure != Figure::Empty;
}

constexpr bool Turn::isAttack() const noexcept
{
    return m_attack;
}

constexpr void Turn::setFigure(Figure figure)
{
    m_figure = figure;
}
constexpr Figure Turn::figure() const noexcept
{
    return Figure{(int8_t) m_figure};
}
