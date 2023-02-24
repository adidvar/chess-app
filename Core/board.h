#ifndef BOARD_H
#define BOARD_H
#include <cinttypes>

#include <array>
#include <list>
#include <vector>

#include "positions.h"
#include "figures.h"
#include "turn.h"

/**
 * \brief Клас доски який описує позиції фігур і надає інтерфейс для доступу читання
 *
 * Клас зберігає в собі шахматну доску
 * також містить інформацію про рокіровки і дані для побиття на проході
 */
class Board
{
    static const char* kStartPosition_;

    enum MatchState
    {
        kActiveNow, ///< Матч продовжується
        kWhiteCheckmate, ///< Білі зловили мат
        kBlackCheckmate, ///< Чорні зловили мат
        kTie, ///< Нічия
        kWhiteSurrendered, ///<Білі здалися
        kBlackSurrendered ///<Чорні здалися
    } state_;

    struct Cell
    {
        Figure type:4;
        Color color:4;
    };

    struct RookingFlags_t
    {
        bool white_ooo;
        bool white_oo;
        bool black_ooo;
        bool black_oo;
    };

    std::array<Cell,64> board_;

    RookingFlags_t rooking_flags_;
    /**
     * @brief Задає флаги можливості виконання рокірування
     */
    /**
     * @brief Рахує ходи без побиття
     */
    size_t passive_turn_counter_;
    /// номер ходу який буде відбуватся
    size_t turn_counter_;
    /**
     * @brief Задає колір гравця який зарас ходить
     */
    Color current_player_color_;
    /**
     * @brief Використовується для побиття на проході
     * Якщо останній хід був пішкою на 2 то сюди записується координата пішки.
     * Якщо останній хід був не пішкою то сюди записується error_pos
     */
    Position last_pawn_move_;

protected:
    void Set(Position position , Cell cell); ///< Записує фігуру
    void Swap(Position p1 , Position p2); ///< Змінює фігури місцями
    void UpdateState();

    std::vector<Turn> GenerateTurns(Color color) const;
public:
    Board(std::string_view fen_line = kStartPosition_); ///< fen парсер карт

    Board& operator =(const Board& b) noexcept = default; ///<Оператор присвоєння
    Board(const Board&) noexcept = default;
    Board( Board&&) noexcept = default;

    Color CurrentColor() const noexcept;
    RookingFlags_t RookingFlags() const noexcept;
    size_t TurnCounter() const noexcept;
    size_t PassiveTurnCounter() const noexcept;

    bool Test(Figure figure , Position position) const noexcept; ///< Порівнює фігури в заданних координатах
    bool TestColor(Color color ,Position position) const noexcept; ///< Порівнює колір в заданих координатах
    bool TestEmp(Position position) const noexcept; ///< Перевіряє клітинку на пустоту

    Cell GetCell(Position position) const noexcept; ///< Повертає фігуру по координатах
    Figure GetFigure(Position position) const noexcept; ///< Повертає фігуру по координатах
    Color GetColor(Position position) const noexcept; ///< Повертає колір по координатах

    bool MateTest() const;
    bool End() const;
    bool Checkmate() const;
    bool WhiteWin() const;
    bool BlackWin() const;
    bool Tie() const;

    std::vector<Turn> GenerateTurns() const;
    /**
 * @brief Виконує хід на дошці
 * @param board дошка
 * @param turn хід
 * @return чи виконаний був хід
 */
    bool ExecuteTurn(Turn turn);
    /**
 * @brief Перевіряє хід на правильність
 * @param board дошка
 * @param turn хід
 * @return flag правильності
 */
    bool TestTurn(Turn turn) const;

   // friend bool ExecuteTurn(Board &board, Turn turn);
};

#endif // BOARD_H
