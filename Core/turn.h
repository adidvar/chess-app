#ifndef TURN_H
#define TURN_H
#include <string>

#include "figures.h"
#include "positions.h"

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
    bool operator ==(const Turn& turn);

    Position from() const noexcept;
    Position to() const noexcept;
    Figure figure() const noexcept;

    std::string ToChessFormat() const;
};

#endif // TURN_H
