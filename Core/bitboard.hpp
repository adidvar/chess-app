#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <array>
#include <cinttypes>
#include <list>
#include <vector>

#include "boardtools.hpp"
#include "position.hpp"
#include "figures.hpp"
#include "turn.hpp"

/**
 * @brief class is board representation using bitboards
 *
 */
class BitBoard
{
    static const char* kStartPosition_;

    friend class FenLoader<BitBoard>;
    friend class FenSaver<BitBoard>;

    using bitboard_t = uint64_t;

    struct Cell
    {
        Figure type;
        Color color;
    };

    struct RookingFlags_t
    {
        bool white_ooo:1;
        bool white_oo:1;
        bool black_ooo:1;
        bool black_oo:1;
    };

    //bitboards
    bitboard_t board_[2][7];
    bitboard_t all_[2];

    //additional state
    RookingFlags_t rooking_flags_;
    uint16_t passive_turn_counter_;
    uint16_t turn_counter_;
    Color current_player_color_;
    Position last_pawn_move_;

    void GenerateSubBoards(Color color, std::vector<BitBoard>&) const;

    void Move(uint64_t from , uint64_t to, Color color, Figure type); // recalculates all_ and empty bitboads
    void Attack(uint64_t from , uint64_t to, Color color, Figure type); // recalculates all_ and empty bitboads
    void Transform(uint64_t sq , Color color, Figure from, Figure to); // recalculates all_ and empty bitboads

    template<typename Type>
    void ProcessFigure(const BitBoard &parrent, std::vector<BitBoard> &boards, Color color, uint64_t from_mask, uint64_t to_mask, uint64_t all, uint64_t yours, uint64_t opponent) const;
    template<typename Type>
    uint64_t ProcessAttack(Color color, uint64_t from_mask, uint64_t all, uint64_t yours, uint64_t opponent) const;

    bool OpponentMateTest() const;
    uint64_t AttackMask(Color color) const;
public:
    /**
     * @brief BitBoard construct class using fen string
     * @param fen_line string with fen
     */
    BitBoard(std::string_view fen_line = kStartPosition_); ///< fen парсер карт
    BitBoard& operator =(const BitBoard& b) noexcept = default; ///<Оператор присвоєння

    /**
     * @brief Set set a figure on a place
     * @param position where we place
     * @param cell what we place
     */
    void Set(Position position, Cell cell); ///< Записує фігуру
    /**
     * @brief Swap swaps two figures
     */
    void Swap(Position p1 , Position p2); ///< Змінює фігури місцями
    /**
     * @brief SkipMove skips current move
     */
    void SkipMove(); //< Пропускає хід

    /**
     * @brief Fen return fen line of a board
     * @return fen line
     */
    std::string Fen() const;

    BitBoard(const BitBoard&) noexcept = default;
    BitBoard( BitBoard&&) noexcept = default;

    /**
     * @brief CurrentColor return current color
     * @return current color
     */
    Color CurrentColor() const noexcept;
    /**
     * @brief OpponentColor return opponent color
     * @return opponent color
     */
    Color OpponentColor() const noexcept;
    /**
     * @brief RookingFlags return flags of possible castling
     * @return struct with flags
     */
    RookingFlags_t RookingFlags() const noexcept;
    /**
     * @brief TurnCounter returns count of turns maked on board
     * @return count of turns from first position
     */
    size_t TurnCounter() const noexcept;
    /**
     * @brief PassiveTurnCounter return count of half turns from begining
     * @return count of half turns
     */
    size_t PassiveTurnCounter() const noexcept;
    /**
     * @brief LastPawnMove return last pawn move for el passant
     * @return position, that last pawn move go through
     */
    Position LastPawnMove() const noexcept;

    /**
     * @brief Test test if figure on position is figure
     * @param position position on which we testing
     */
    bool Test(Figure figure , Position position) const noexcept; ///< Порівнює фігури в заданних координатах
    /**
     * @brief TestColor test is color on position is color
     * @attention color if empty figure is undefined
     * @param position position on which we testing
     */
    bool TestColor(Color color ,Position position) const noexcept; ///< Порівнює колір в заданих координатах
    /**
     * @brief TestEmp test if figure on position is empty
     * @param position position on which we testing
     * @return
     */
    bool TestEmp(Position position) const noexcept; ///< Перевіряє клітинку на пустоту

    /**
     * @brief GetFigure return figure from position
     * @return figure
     */
    Figure GetFigure(Position position) const noexcept; ///< Повертає фігуру по координатах
    /**
     * @brief GetColor return color from position
     * @return color
     */
    Color GetColor(Position position) const noexcept; ///< Повертає колір по координатах
    /**
     * @brief GetCell return cell from position
     * @return cell
     */
    Cell GetCell(Position position) const noexcept;

    /**
     * @brief MateTest tests mate on current board
     */
    bool MateTest() const;
    /**
     * @brief End tests that game can continue
     */
    bool End() const;
    /**
     * @brief Checkmate tests checkmate on current board
     * @return
     */
    bool Checkmate() const;
    /**
     * @brief WhiteWin tests white checkmate
     * @return
     */
    bool WhiteWin() const;
    /**
     * @brief BlackWin tests black checkmate
     * @return
     */
    bool BlackWin() const;
    /**
     * @brief Tie returns true if there is a tie
     * @return
     */
    bool Tie() const;

    /**
     * @brief GenerateSubBoards returns all possible subboards
     */
    std::vector<BitBoard> GenerateSubBoards() const;
    void GenerateSubBoards(std::vector<BitBoard> &boards) const;

    /**
     * @brief GenerateTurns returns all possible turns
     */
    std::vector<Turn> GenerateTurns() const;
    /**
     * @brief ExecuteTurn executes turn on these board
     * @param turn our turn
     * @return return false if error happens
     */
    bool ExecuteTurn(Turn turn);
    /**
     * @brief TestTurn tests turn correctness
     * @return true if ok, otherwise false
     */
    bool TestTurn(Turn turn) const;
};

#endif // BOARD_H
