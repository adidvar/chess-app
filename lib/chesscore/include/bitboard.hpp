#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <array>
#include <cinttypes>
#include <list>
#include <vector>

#include "bitconsts.hpp"
#include "figures.hpp"
#include "position.hpp"
#include "turn.hpp"

struct BitBoardTuple;
/**
 * @brief class of a board implemented with magic bitboards
 */
class BitBoard {
 public:
  /**
   * @brief The Cell class contains info about a bitboard cell
   */
  struct Cell {
    Figure type;
    Color color;
  };
  /**
   * @brief The RookingFlagsT class contains info about rooking state of
   * bitboard
   */
  struct RookingFlagsT {
    bool white_ooo : 1;
    bool white_oo : 1;
    bool black_ooo : 1;
    bool black_oo : 1;
  };
  /**
   * @brief AttackMask return mask of positions we can move or attack
   * @param color color of figures we use
   * @return mask
   */
  [[nodiscard]] bitboard_t AttackMask(Color color) const;
  /**
   * @brief BitBoard construct class with startpos
   */
  BitBoard();
  /**
   * @brief BitBoard construct class with fen line
   * @param fen_line fen line
   */
  BitBoard(std::string_view fen_line);
  /**
   * @brief operator = default operator =
   */
  // BitBoard &operator=(const BitBoard &b) noexcept = default;
  /**
   * @brief Set set a figure on a place
   * @param position where we place
   * @param cell what we place
   */
  void Set(Position position, Cell cell);
  /**
   * @brief SetCurrentColor set current color variable
   */
  void SetCurrentColor(Color color);
  /**
   * @brief SetRookingFlags set rooking flags state
   */
  void SetRookingFlags(RookingFlagsT flags);
  /**
   * @brief SetLastPawnMove set el passant state
   */
  void SetLastPawnMove(Position last_pawn_move);
  /**
   * @brief Swap swaps two figures
   */
  void Swap(Position p1, Position p2);
  /**
   * @brief SkipMove skips current move
   */
  void SkipMove();
  /**
   * @brief Fen return fen line of a board
   * @return fen line
   */
  [[nodiscard]] std::string Fen() const;
  /**
   * @brief CurrentColor return current color
   * @return current color
   */
  [[nodiscard]] Color CurrentColor() const noexcept;
  /**
   * @brief OpponentColor return opponent color
   * @return opponent color
   */
  [[nodiscard]] Color OpponentColor() const noexcept;
  /**
   * @brief RookingFlags return flags of possible castling
   * @return struct with flags
   */
  [[nodiscard]] RookingFlagsT RookingFlags() const noexcept;
  /**
   * @brief LastPawnMove return last pawn move for el passant
   * @return position, that last pawn move go through
   */
  [[nodiscard]] Position LastPawnMove() const noexcept;
  /**
   * @brief Test test if figure on position is figure
   * @param position position on which we testing
   */
  [[nodiscard]] bool Test(Figure figure, Position position)
      const noexcept;  ///< Порівнює фігури в заданних координатах
  /**
   * @brief TestColor test is color on position is color
   * @attention color if empty figure is undefined
   * @param position position on which we testing
   */
  [[nodiscard]] bool TestColor(Color color, Position position)
      const noexcept;  ///< Порівнює колір в заданих координатах
  /**
   * @brief TestEmp test if figure on position is empty
   * @param position position on which we testing
   * @return
   */
  [[nodiscard]] bool TestEmp(
      Position position) const noexcept;  ///< Перевіряє клітинку на пустоту
  /**
   * @brief GetFigure return figure from position
   * @return figure
   */
  [[nodiscard]] Figure GetFigure(
      Position position) const noexcept;  ///< Повертає фігуру по координатах
  /**
   * @brief GetColor return color from position
   * @return color
   */
  [[nodiscard]] Color GetColor(
      Position position) const noexcept;  ///< Повертає колір по координатах
  /**
   * @brief GetCell return cell from position
   * @return cell
   */
  [[nodiscard]] Cell GetCell(Position position) const noexcept;
  /**
   * @brief GetBitBoard returns the raw bitboard of figure
   * @param color color
   * @param figure figure
   * @return bitboard
   */
  [[nodiscard]] bitboard_t GetBitBoard(Color color,
                                       Figure figure) const noexcept;
  /**
   * @brief GetColorBitBoard returns raw bitboard of player all figure
   * @param color color of player
   * @return bitboard
   */
  [[nodiscard]] bitboard_t GetColorBitBoard(Color color) const noexcept;
  /**
   * @brief MateTest tests mate on current board
   */
  [[nodiscard]] bool MateTest() const;
  /**
   * @brief End tests that game can continue
   */
  [[nodiscard]] bool End() const;
  /**
   * @brief Checkmate tests checkmate on current board
   * @return
   */
  [[nodiscard]] bool Checkmate() const;
  /**
   * @brief WhiteWin tests white checkmate
   * @return
   */
  [[nodiscard]] bool WhiteWin() const;
  /**
   * @brief BlackWin tests black checkmate
   * @return
   */
  [[nodiscard]] bool BlackWin() const;
  /**
   * @brief Tie returns true if there is a tie
   * @return
   */
  [[nodiscard]] bool Tie() const;
  /**
   * @brief GenerateSubBoards returns all valid subboards
   * @param color color
   * @param from from mask
   * @param to to mask
   * @return vector of bitboards
   */
  [[nodiscard]] std::vector<BitBoard> GenerateSubBoards(
      Color color, uint64_t from = kall, uint64_t to = kall) const;
  /**
   * @brief GenerateSubBoards returns all valid subboards
   * @param boards vector for boards placement
   * @param color color
   * @param from from mask
   * @param to to mask
   */
  void GenerateSubBoards(std::vector<BitBoard> &boards, Color color,
                         uint64_t from = kall, uint64_t to = kall) const;
  /**
   * @brief GenerateTurns returns all possible turns
   * @param color color
   * @param from from mask
   * @param to to mask
   * @return vector of turns
   */
  [[nodiscard]] std::vector<Turn> GenerateTurns(Color color,
                                                uint64_t from = kall,
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
  [[nodiscard]] bool TestTurn(Turn turn) const;
  /**
   * @brief Hash returns zobrist hash of bitboard
   * @return hash
   *
   * to see a realization go zobrist.hpp
   */
  [[nodiscard]] bitboard_hash_t Hash() const;
  /**
   * @brief GenerateTuplesFast generates subboards subturns and subhashes
   * @param tuple input tuple
   * @param color color
   * @param from from mask
   * @param to to mask
   * @return array of tuples
   * @attention function is much faster than split generation
   */
  static std::vector<BitBoardTuple> GenerateTuplesFast(BitBoardTuple tuple,
                                                       Color color,
                                                       uint64_t from = kall,
                                                       uint64_t to = kall);
  /**
   * @brief GenerateTurn generates a turn from two boards
   * @param from board before turn
   * @param to board after turn
   * @param color color of current player
   * @return turn
   */
  static Turn GenerateTurn(const BitBoard &from, const BitBoard &to,
                           Color color);
  /**
   * @brief GenerateHash generates hash from a board its hash and a turn
   * @param board a board
   * @param the hash hash of board
   * @param turn the turn
   * @return the hash of sub board
   */
  static bitboard_hash_t GenerateHash(const BitBoard &board,
                                      bitboard_hash_t hash, Turn turn,
                                      const BitBoard &sub);
  /**
   * @brief GenerateTurns generates a vector of turns
   * @param main the root node
   * @param subboards subboards
   * @param color a color
   * @return the vector of turns
   */
  static std::vector<Turn> GenerateTurns(const BitBoard &main,
                                         const std::vector<BitBoard> &subboards,
                                         Color color);
  /**
   * @brief GenerateHashes generate a vector of hashes
   * @param main the root node
   * @param subboards subboards
   * @return the vector of turns
   */
  static std::vector<bitboard_hash_t> GenerateHashes(
      const BitBoard &main, const std::vector<BitBoard> &subboards);

  bool operator==(const BitBoard &board) const;
  bool operator!=(const BitBoard &board) const;

 private:
  static const char *const kStartPosition;
  static const BitBoard kStartBitBoard;

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
  [[nodiscard]] bitboard_t ProcessAttack(Color color, bitboard_t from_mask,
                                         bitboard_t all, bitboard_t yours,
                                         bitboard_t opponent) const;

  [[nodiscard]] bool OpponentMateTest() const;
};

struct BitBoardTuple {
  BitBoardTuple(const BitBoard &board)
      : board(board), hash(board.Hash()), turn{}, priority{} {}

  BitBoardTuple(const BitBoard &board, bitboard_hash_t hash, Turn turn)
      : board(board), hash(hash), turn{turn}, priority{} {}

  BitBoard board;
  bitboard_hash_t hash;
  Turn turn;

  struct Priority {
    int64_t type;
    int64_t index;
  } priority;

  bool operator<(const BitBoardTuple &t1) const noexcept {
    return std::tie(priority.type, priority.index) <
           std::tie(t1.priority.type, t1.priority.index);
  }
};

#endif  // BOARD_H
