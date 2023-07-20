#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <array>
#include <cinttypes>
#include <list>
#include <vector>

#include "figures.hpp"
#include "position.hpp"
#include "turn.hpp"

using bitboard_t = uint64_t;
using bitboard_hash_t = uint64_t;

constexpr bitboard_t operator""_b(bitboard_t num) {
  return (bitboard_t)1 << num;
}

constexpr bitboard_t PositionToBitMask(bitboard_t num) {
  return (bitboard_t)1 << num;
}

constexpr bitboard_t PositionToBitMask(Position position) {
  return (bitboard_t)1 << position.Value();
}

struct BitBoardTuple;

struct BitIterator {
  bitboard_t value_;
  bitboard_t bit_;

  constexpr BitIterator(bitboard_t value) : value_(value), bit_(0) {
    operator++();
  }

  constexpr bitboard_t Value() const { return value_; }
  constexpr bitboard_t Bit() const { return bit_; }
  constexpr void operator=(bitboard_t value) {
    value_ = value;
    operator++();
  }
  constexpr void operator++() {
    bit_ = value_ ^ ((value_ - 1) & value_);
    value_ &= ~bit_;
  }
  constexpr bool Valid() { return bit_ != 0; }
};

constexpr static bitboard_t kall = ~(bitboard_t(0));

/**
 * @brief class is board representation using bitboards
 *
 */
class BitBoard {
 public:
  struct Cell {
    Figure type;
    Color color;
  };

  struct RookingFlagsT {
    bool white_ooo : 1;
    bool white_oo : 1;
    bool black_ooo : 1;
    bool black_oo : 1;
  };

 private:
  static const char *kStartPosition_;
  static const BitBoard kStartBitBoard_;

  // bitboards
  bitboard_t board_[Color::Max()][Figure::Max()];
  bitboard_t all_[Color::Max()];

  // additional state
  RookingFlagsT rooking_flags_;
  Color current_player_color_;
  Position last_pawn_move_;

  void GenerateSubBoards(Color color, std::vector<BitBoard> &,
                         uint64_t from = kall, uint64_t to = kall) const;

  void Move(bitboard_t from, bitboard_t to, Color color,
            Figure type);  // recalculates all_ and empty bitboads
  void Attack(bitboard_t from, bitboard_t to, Color color,
              Figure type);  // recalculates all_ and empty bitboads
  void Transform(bitboard_t sq, Color color, Figure from,
                 Figure to);  // recalculates all_ and empty bitboads

  template <typename Type>
  void ProcessFigure(const BitBoard &parrent, std::vector<BitBoard> &boards,
                     Color color, bitboard_t from_mask, bitboard_t to_mask,
                     bitboard_t all, bitboard_t yours,
                     bitboard_t opponent) const;

  template <typename Type>
  bitboard_t ProcessAttack(Color color, bitboard_t from_mask, bitboard_t all,
                           bitboard_t yours, bitboard_t opponent) const;

  bool OpponentMateTest() const;

 public:
  bitboard_t AttackMask(Color color) const;
  /**
   * @brief BitBoard construct class using fen string
   * @param fen_line string with fen
   */
  BitBoard();                           ///< fen парсер карт
  BitBoard(std::string_view fen_line);  ///< fen парсер карт
  ~BitBoard() = default;
  BitBoard &operator=(const BitBoard &b) noexcept =
      default;  ///< Оператор присвоєння
  /**
   * @brief Set set a figure on a place
   * @param position where we place
   * @param cell what we place
   */
  void Set(Position position, Cell cell);  ///< Записує фігуру

  void SetCurrentColor(Color color);

  void SetRookingFlags(RookingFlagsT flags);

  void SetLastPawnMove(Position lp);

  /**
   * @brief Swap swaps two figures
   */
  void Swap(Position p1, Position p2);  ///< Змінює фігури місцями
  /**
   * @brief SkipMove skips current move
   */
  void SkipMove();  //< Пропускає хід

  /**
   * @brief Fen return fen line of a board
   * @return fen line
   */
  std::string Fen() const;

  BitBoard(const BitBoard &) noexcept = default;
  BitBoard(BitBoard &&) noexcept = default;

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
  RookingFlagsT RookingFlags() const noexcept;
  /**
   * @brief LastPawnMove return last pawn move for el passant
   * @return position, that last pawn move go through
   */
  Position LastPawnMove() const noexcept;
  /**
   * @brief Test test if figure on position is figure
   * @param position position on which we testing
   */
  bool Test(Figure figure, Position position)
      const noexcept;  ///< Порівнює фігури в заданних координатах
  /**
   * @brief TestColor test is color on position is color
   * @attention color if empty figure is undefined
   * @param position position on which we testing
   */
  bool TestColor(Color color, Position position)
      const noexcept;  ///< Порівнює колір в заданих координатах
  /**
   * @brief TestEmp test if figure on position is empty
   * @param position position on which we testing
   * @return
   */
  bool TestEmp(
      Position position) const noexcept;  ///< Перевіряє клітинку на пустоту
  /**
   * @brief GetFigure return figure from position
   * @return figure
   */
  Figure GetFigure(
      Position position) const noexcept;  ///< Повертає фігуру по координатах
  /**
   * @brief GetColor return color from position
   * @return color
   */
  Color GetColor(
      Position position) const noexcept;  ///< Повертає колір по координатах
  /**
   * @brief GetCell return cell from position
   * @return cell
   */
  Cell GetCell(Position position) const noexcept;

  bitboard_t GetBitBoard(Color color, Figure figure) const noexcept;
  bitboard_t GetColorBitBoard(Color color) const noexcept;
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
  std::vector<BitBoard> GenerateSubBoards(Color color, uint64_t from = kall,
                                          uint64_t to = kall) const;
  void GenerateSubBoards(std::vector<BitBoard> &boards, Color color,
                         uint64_t from = kall, uint64_t to = kall) const;
  /**
   * @brief GenerateTurns returns all possible turns
   */
  std::vector<Turn> GenerateTurns(Color color, uint64_t from = kall,
                                  uint64_t to = kall) const;
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

  bitboard_hash_t Hash() const;

  static std::vector<BitBoardTuple> GenerateTuplesFast(BitBoardTuple tuple,
                                                       Color color,
                                                       uint64_t from = kall,
                                                       uint64_t to = kall);

  static Turn GetTurn(const BitBoard &from, const BitBoard &to, Color color);
  static bitboard_hash_t GetHash(const BitBoard &board, bitboard_hash_t hash,
                                 Turn turn);
  static std::vector<Turn> GenerateTurns(const BitBoard &main,
                                         const std::vector<BitBoard> &subboards,
                                         Color color);
  static std::vector<bitboard_hash_t> GenerateHashes(
      const BitBoard &main, const std::vector<BitBoard> &subboards);

  bool operator==(const BitBoard &board) const;
  bool operator!=(const BitBoard &board) const;
};

struct BitBoardTuple {
  BitBoard board;
  bitboard_hash_t hash;
  Turn turn;
};

#endif  // BOARD_H
