#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <list>
#include "positions.h"
#include "figures.h"

/**
 * \brief Клас доски який описує позиції фігур і надає інтерфейс для доступу читання
 *
 * Клас зберігає в собі шахматну доску
 * також містить інформацію про рокіровки і дані для побиття на проході
 */

class Turn;
class Board;

class Board
{
    static const char* fen_start;
    struct figure
    {
        figure():
            type(empty),
            color(white)
        {}
        uint8_t type:3;
        uint8_t color:1;
    };
    std::array<figure,64> map;

    /**
     * @brief Задає флаги можливості виконання рокірування
     */
    struct rooking_flags
    {
        bool white_oo;
        bool white_ooo;
        bool black_oo;
        bool black_ooo;
    } r_flags;

    /**
     * @brief Використовується для побиття на проході
     * Якщо останній хід був пішкою на 2 то сюди записується координата пішки.
     * Якщо останній хід був не пішкою то сюди записується error_pos
     */
    Pos last_pawn_move = error_pos;
    /**
     * @brief Рахує ходи без побиття
     */
    size_t static_move_counter = 0;
    /// номер ходу який буде відбуватся
    size_t turn_counter = 1;

    /**
     * @brief Задає колір гравця який зарас ходить
     */
    bool current_color;

protected:
    void Set(Pos position , Figures figure , Color c ); ///< Записує фігуру
    void Swap(Pos p1 , Pos p2); ///< Змінює фігури місцями
public:
    Board(const std::initializer_list<std::pair<Figures,Color>> list); ///< Ініціалізація доски з файлу
    Board(std::string_view fen = fen_start); ///< fen парсер карт


    Board& operator =(const Board& b) = default; ///<Оператор присвоєння
    Board(const Board&) = default;
    Board( Board&&) = default;

    Color CurrentColor() const {return current_color;};
    rooking_flags RookingState() const {return r_flags;};

    std::pair<size_t , size_t> TurnCounters(){return {turn_counter , static_move_counter};};

    bool Test(Figures figure , Pos position) const; ///< Порівнює фігури в заданних координатах
    bool TestColor(Color color ,Pos position) const; ///< Порівнює колір в заданих координатах
    bool TestEmp(Pos position) const; ///< Перевіряє клітинку на пустоту

    Figures GetFigure(Pos position) const; ///< Повертає фігуру по координатах
    Color GetColor(Pos position) const; ///< Повертає колір по координатах

    operator bool();

    friend bool ExecuteTurn(Board &board, Turn turn);
};

#endif // BOARD_H
