#pragma once

#include <string>

#include "figures.hpp"
#include "position.hpp"

class Turn {
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

     constexpr [[nodiscard]] Position from() const noexcept;
     constexpr [[nodiscard]] Position to() const noexcept;
     constexpr [[nodiscard]] Figure figure() const noexcept;

     constexpr [[nodiscard]] bool isValid() const noexcept;
     [[nodiscard]] bool isCastling() const noexcept;
     [[nodiscard]] bool isLongCastling() const noexcept;
     [[nodiscard]] bool isShortCastling() const noexcept;
     constexpr [[nodiscard]] bool isTrasformation() const noexcept;
     constexpr [[nodiscard]] bool isAttack() const noexcept;

     static Turn getShortCastling(Color color);
     static Turn getLongCastling(Color color);

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
{
}

constexpr Turn::Turn(Position from, Position to, Figure figure, bool attack)
    : m_from(from.index())
    , m_to(to.index())
    , m_figure(figure)
    , m_attack(attack)
{
}

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
