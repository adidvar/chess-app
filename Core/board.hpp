#ifndef BOARD_H
#define BOARD_H
#include <cinttypes>

#include <array>
#include <list>
#include <vector>

#include "position.hpp"
#include "figures.hpp"
#include "turn.hpp"
#include "boardtools.hpp"

/**
 * \brief Клас доски який описує позиції фігур і надає інтерфейс для доступу читання
 *
 * Клас зберігає в собі шахматну доску
 * також містить інформацію про рокіровки і дані для побиття на проході
 */
class Board
{
    static const char* kStartPosition_;

    friend class FenLoader<Board>;
    friend class FenSaver<Board>;

    struct Cell
    {
        Figure type;
        Color color;
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
    size_t passive_turn_counter_;
    size_t turn_counter_;
    Color current_player_color_;
    Position last_pawn_move_;
protected:
    void UnsafeExecuteTurn(Turn turn);
    std::vector<Turn> UnsafeTurns(Color color) const;
    std::vector<Turn> GenerateTurns(Color color) const;

public:
    Board(std::string_view fen_line = kStartPosition_); ///< fen парсер карт
    Board& operator =(const Board& b) noexcept = default; ///<Оператор присвоєння

    void Set(Position position, Cell cell); ///< Записує фігуру
    void Swap(Position p1 , Position p2); ///< Змінює фігури місцями
    [[deprecated]]
    void SkipMove(); //< Пропускає хід

    std::string Fen() const;

    Board(const Board&) noexcept = default;
    Board( Board&&) noexcept = default;

    Color CurrentColor() const noexcept;
    Color OpponentColor() const noexcept;
    RookingFlags_t RookingFlags() const noexcept;
    size_t TurnCounter() const noexcept;
    size_t PassiveTurnCounter() const noexcept;
    Position LastPawnMove() const noexcept;

    bool Test(Figure figure , Position position) const noexcept; ///< Порівнює фігури в заданних координатах
    bool TestColor(Color color ,Position position) const noexcept; ///< Порівнює колір в заданих координатах
    bool TestEmp(Position position) const noexcept; ///< Перевіряє клітинку на пустоту

    Figure GetFigure(Position position) const noexcept; ///< Повертає фігуру по координатах
    Color GetColor(Position position) const noexcept; ///< Повертає колір по координатах
    Cell GetCell(Position position) const noexcept;

    bool UnderAtack(Position position) const;
    bool MateTest() const;
    bool End() const;
    bool Checkmate() const;
    bool WhiteWin() const;
    bool BlackWin() const;
    bool Tie() const;

    std::vector<Board> GenerateSubBoards() const;

    std::vector<Turn> GenerateTurns() const;
    bool ExecuteTurn(Turn turn);
    bool TestTurn(Turn turn) const;
};

#endif // BOARD_H
