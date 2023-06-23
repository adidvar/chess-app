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

class alignas(4) Turn
{
    Position from_;
    Position to_;
    Figure figure_;

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
    Turn(Position from, Position to , Figure figure);
    /**
     * @brief Рокірування
     * @param direction напрямок рокіровки
     */

    Turn(const Turn&)noexcept = default;
    Turn(Turn&&) noexcept = default;

    Turn& operator =(const Turn& b) noexcept = default;
    Turn& operator =(Turn&& b) noexcept = default;
    bool operator ==(const Turn& turn) const;
    bool operator <(const Turn& turn) const;

    Position from() const noexcept;
    Position to() const noexcept;
    Position &from() noexcept;
    Position &to()  noexcept;
    Figure figure() const noexcept;
    Figure &figure() noexcept;

    bool Valid() const noexcept;

    bool IsCastling() const noexcept;
    bool IsLongCastling() const noexcept;
    bool IsShortCastling() const noexcept;

    bool IsTrasformation() const noexcept;

    std::string ToChessFormat() const;
    static Turn FromChessFormat(std::string_view string);
    static Turn GetShortCastling(Color color);
    static Turn GetLongCastling(Color color);
};

#endif // TURN_H
