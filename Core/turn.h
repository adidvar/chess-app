#ifndef TURN_H
#define TURN_H
#include "board.h"
#include <variant>
#include <tuple>
#include <vector>
#include <optional>
#include <string>

/**
 * @brief Визначає звичайний шахматний
 *
 * Ходи бувають одного з 3 типів
 * 1) Звичайних хід
 * 2) Хід пішкою з вибором фігури
 * 3) Рокірування (false -- коротка , true -- довга)
 */

using Rooking = bool;
enum Rooking_t
{
    rooking_oo =0,
    rooking_ooo=1
};

class Turn
{
    using variant_type = std::variant<std::monostate,
    std::tuple<Pos,Pos>,
    std::tuple<Pos,Pos,Figures>,Rooking>;

    variant_type turn;
public:
    /**
     * @brief Пустий хід
     */
    Turn() = default;

    /**
     * @brief Звичайний хід
     * @param move задає початкову і кінцеву позицію
     */
    Turn(std::tuple<Pos,Pos> move);
    /**
     * @brief Кінцевий хід пішкою
     * @param pawn_move Початкова кінцева і тип фігури
     */
    Turn(std::tuple<Pos,Pos,Figures> pawn_move);
    /**
     * @brief Рокірування
     * @param direction напрямок рокіровки
     */
    Turn(Rooking direction);

    Turn(const Turn&) = default;
    Turn(Turn&&) = default;

    Turn& operator =(const Turn& b) = default;
    Turn& operator =(Turn&& b) = default;

    variant_type variant() const {return this->turn;};

    std::optional<std::tuple<Pos,Pos>> GetTurn() const;
    std::optional<std::tuple<Pos,Pos,Figures>> GetPawnTurn() const;
    std::optional<Rooking> GetRooking() const;

    std::string toChessFormat() const;

    bool operator ==(const Turn &t) const;

friend bool ExecuteTurn(Board &board, Turn turn);
};

#endif // TURN_H
