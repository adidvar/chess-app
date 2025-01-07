#include "bitboard.hpp"

#include "bitutils.hpp"
#include "fen.hpp"
#include "magic.hpp"

const char *const BitBoard::kStartPosition
    = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const BitBoard BitBoard::kStartBitBoard{BitBoard::kStartPosition};

BitBoard::BitBoard() : BitBoard(kStartBitBoard) {}

BitBoard::BitBoard(std::string_view fen)
{
    size_t index = 0;
    boardFromFen(fen, *this, index);
}

std::string BitBoard::fen() const
{
    return boardToFen(*this);
}

/*
void BitBoard::generateSubBoards(Color color, std::vector<BitBoard> &boards,
                                 uint64_t from, uint64_t to) const {
  boards.clear();

  bitboard empty = board_[color][Figure::Empty];
  bitboard opponent = all_[!color];
  bitboard yours = all_[color];
  bitboard all = ~empty;
  bitboard attack = AttackMask(!color);

  BitBoard parent(*this);
  parent.last_pawn_move_ = Position();
  parent.SkipMove();

  bitboard locked_figures = attack & yours & attackFrom(board_[color][Figure::WKing]);

  // queen
  ProcessFigure<Queen>(parent, boards, color, ~locked_figures & from, to, all,
                       yours, opponent);
  // rook
  ProcessFigure<Rook>(parent, boards, color, ~locked_figures & from, to, all,
                      yours, opponent);
  // knight
  ProcessFigure<Knight>(parent, boards, color, ~locked_figures & from, to, all,
                        yours, opponent);
  // bishop
  ProcessFigure<Bishop>(parent, boards, color, ~locked_figures & from, to, all,
                        yours, opponent);
  // pawns
  ProcessFigure<Pawn>(parent, boards, color, ~locked_figures & from, to, all,
                      yours, opponent);
  // king
  ProcessFigure<King>(parent, boards, color, ~locked_figures & from,
                      ~attack & to, all, yours, opponent);

  auto index = boards.size();
  if ((attack & board_[color][Figure::WKing]) != 0)
      index = 0;

  // queen
  ProcessFigure<Queen>(parent, boards, color, locked_figures & from, to, all,
                       yours, opponent);
  // rook
  ProcessFigure<Rook>(parent, boards, color, locked_figures & from, to, all,
                      yours, opponent);
  // knight
  ProcessFigure<Knight>(parent, boards, color, locked_figures & from, to, all,
                        yours, opponent);
  // bishop
  ProcessFigure<Bishop>(parent, boards, color, locked_figures & from, to, all,
                        yours, opponent);
  // pawns
  ProcessFigure<Pawn>(parent, boards, color, locked_figures & from, to, all,
                      yours, opponent);
  // king
  ProcessFigure<King>(parent, boards, color, locked_figures & from,
                      ~attack & to, all, yours, opponent);

}
*/

#include <iostream>

template<BitBoard::Flags flags>
class BitBoardHelper
{
    BitBoard board;
    Turn *storage;

public:
    using bitboard = BitBoard::bitboard;

    constexpr BitBoardHelper(const BitBoard &board, Turn *storage)
        : board(board)
        , storage(storage)
    {}
    template<int8_t shift>
    constexpr bitboard pawnsShift(bitboard in)
    {
        if constexpr (shift > 0) {
            if constexpr ((flags & BitBoard::flags_color) == 0)
                return in >> shift;
            else
                return in << shift;
        } else {
            if constexpr ((flags & BitBoard::flags_color) == 0)
                return in << -shift;
            else
                return in >> -shift;
        }
    }
    constexpr bitboard chooseMask(bitboard mask1, bitboard mask2)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return mask1;
        else
            return mask2;
    }

    template<int8_t delta, bool attack, int8_t figure>
    constexpr Turn generateTurnDelta(bitboard to)
    {
        int8_t pos = log2_64(to);
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return {Position(pos + delta), Position(pos)};
        else
            return {Position(pos - delta), Position(pos)};
    }

    template<bool attack, int8_t figure>
    constexpr Turn generateTurn(Position from, Position to)
    {
        return {Position(from), Position(to)};
    }

    constexpr int generateBlockedTurn(
        bitboard from, bitboard to_mask, bitboard enemy, int counter, Turn *out)
    { // here we can use generation without magic
        Position from_position = log2_64(from);
        bitboard figure_move_mask = 0;
        Figure figure = Figure::Empty;
        if (from & getPawns(board))
            figure_move_mask = pawnsShift<8>(from) | pawnsShift<16>(from)
                               | (pawnsShift<9>(from) & enemy) | (pawnsShift<7>(from) & enemy);
        else if (from & getKnights(board))
            return counter;
        else if (from & getBishops(board))
            figure_move_mask = processBishop(from_position, 0);
        else if (from & getRooks(board))
            figure_move_mask = processRook(from_position, 0);
        else if (from & getQueens(board))
            figure_move_mask = processRook(from_position, 0) | processBishop(from_position, 0);
        figure_move_mask &= to_mask;

        for (bitboard bit = takeBit(figure_move_mask); bit; bit = takeBit(figure_move_mask)) {
            Position to_position = log2_64(bit);
            out[counter++] = Turn(from_position, to_position);
        }
        return counter;
    }

    constexpr bitboard getAlies()
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.getWhites();
        else
            return board.getBlacks();
    }
    constexpr bitboard getEnemies()
    {
        if constexpr ((flags & BitBoard::flags_color) != 0)
            return board.getWhites();
        else
            return board.getBlacks();
    }
    constexpr bitboard &getPawns(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_p;
        else
            return board.m_b_p;
    }
    constexpr bitboard &getEnemyPawns(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) != 0)
            return board.m_w_p;
        else
            return board.m_b_p;
    }
    constexpr bitboard &getKnights(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_n;
        else
            return board.m_b_n;
    }
    constexpr bitboard &getEnemyKnights(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) != 0)
            return board.m_w_n;
        else
            return board.m_b_n;
    }
    constexpr bitboard &getKing(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_k;
        else
            return board.m_b_k;
    }
    constexpr bitboard &getEnemyKing(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) != 0)
            return board.m_w_k;
        else
            return board.m_b_k;
    }
    constexpr bitboard &getBishops(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_b;
        else
            return board.m_b_b;
    }
    constexpr bitboard &getEnemyBishops(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) != 0)
            return board.m_w_b;
        else
            return board.m_b_b;
    }
    constexpr bitboard &getRooks(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_r;
        else
            return board.m_b_r;
    }
    constexpr bitboard &getEnemyRooks(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) != 0)
            return board.m_w_r;
        else
            return board.m_b_r;
    }
    constexpr bitboard &getQueens(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_q;
        else
            return board.m_b_q;
    }
    constexpr bitboard &getEnemyQueens(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) != 0)
            return board.m_w_q;
        else
            return board.m_b_q;
    }
    constexpr bitboard getFillBitboard() { return ~((bitboard) 0); }

    constexpr int generate()
    {
        int counter = 0;
        Turn *out = storage;

        bitboard allies = getAlies();
        bitboard enemies = getEnemies();
        bitboard all = allies | enemies;
        bitboard empty = ~all;
        bitboard enemy_attack_mask = 0;

        { /// ATTACK MASK GENERATION
            bitboard borders = all & (~getKing(board));

            enemy_attack_mask |= pawnsShift<-9>(getEnemyPawns(board)) & chooseMask(~row_a, ~row_h);
            enemy_attack_mask |= pawnsShift<-7>(getEnemyPawns(board)) & chooseMask(~row_h, ~row_a);

            bitboard knights = getEnemyKnights(board);
            for (bitboard bit = takeBit(knights); bit; bit = takeBit(knights)) {
                Position from_position = log2_64(bit);
                enemy_attack_mask |= processKnight(from_position);
            }

            bitboard king = getEnemyKing(board);
            for (bitboard bit = takeBit(king); bit; bit = takeBit(king)) {
                Position from_position = log2_64(bit);
                enemy_attack_mask |= processKing(from_position);
            }

            bitboard bishops = getEnemyBishops(board);
            for (bitboard bit = takeBit(bishops); bit; bit = takeBit(bishops)) {
                Position from_position = log2_64(bit);
                enemy_attack_mask |= processBishop(from_position, borders);
            }

            bitboard rooks = getEnemyRooks(board);
            for (bitboard bit = takeBit(rooks); bit; bit = takeBit(rooks)) {
                Position from_position = log2_64(bit);
                enemy_attack_mask |= processRook(from_position, borders);
            }

            bitboard queens = getEnemyQueens(board);
            for (bitboard bit = takeBit(queens); bit; bit = takeBit(queens)) {
                Position from_position = log2_64(bit);
                enemy_attack_mask |= processRook(from_position, borders)
                                     | processBishop(from_position, borders);
            }
        }

        if (bitboard bit = getKing(board); bit) ///MATE PROCESSING
        {
            bitboard diagonal_enemies = getEnemyBishops(board) | getEnemyQueens(board);
            bitboard orthogonal_enemies = getEnemyRooks(board) | getEnemyQueens(board);

            Position king_position = log2_64(bit);
            bitboard k_attack_mask = processKing(king_position);
            bitboard n_attack_mask = processKnight(king_position);
            bitboard b_attack_mask = processBishop(king_position, all);
            bitboard r_attack_mask = processRook(king_position, all);

            bitboard pawns_attack = (pawnsShift<9>(bit) | pawnsShift<7>(bit))
                                    & getEnemyPawns(board);
            bitboard knights_attack = n_attack_mask & getEnemyKnights(board);
            bitboard bishop_attack = b_attack_mask & diagonal_enemies;
            bitboard rook_attack = r_attack_mask & orthogonal_enemies;

            bitboard all_attack = pawns_attack | knights_attack | bishop_attack | rook_attack;

            int attacks_counter = popCount(all_attack);

            bitboard blockers = 0;
            { /// BLOCKERS SEARCH
                bitboard candidates_to_blockers = (b_attack_mask | r_attack_mask) & allies;
                bitboard new_blocker_map = all & (~candidates_to_blockers);

                bitboard new_attackers_map = (processBishop(king_position, new_blocker_map)
                                              & diagonal_enemies)
                                             | (processRook(king_position, new_blocker_map)
                                                & orthogonal_enemies);

                for (bitboard bit = takeBit(new_attackers_map); bit;
                     bit = takeBit(new_attackers_map)) {
                    bitboard way = processWay(king_position, log2_64(bit));
                    bitboard blocker = way & candidates_to_blockers;
                    counter = generateBlockedTurn(blocker, way | bit, bit, counter, out);
                    blockers |= blocker;
                }
            }

            if (attacks_counter == 0)
                //no mate generation
                counter = generateFigures<true>(counter,
                                                empty,
                                                enemies,
                                                all,
                                                ~blockers,
                                                getFillBitboard(),
                                                getFillBitboard());
            else if (attacks_counter == 1) {
                bitboard short_attacks = knights_attack | (all_attack & k_attack_mask);
                int short_attacks_counter = popCount(short_attacks);
                if (short_attacks_counter == 0)
                    // single long mate
                    counter = generateFigures<true>(counter,
                                                    empty,
                                                    enemies,
                                                    all,
                                                    ~blockers,
                                                    processWay(king_position, log2_64(all_attack)),
                                                    all_attack);
                else
                    //single short mate
                    counter = generateFigures<false>(counter,
                                                     empty,
                                                     enemies,
                                                     all,
                                                     ~blockers,
                                                     0,
                                                     all_attack);
            }
        }
        bitboard king = getKing(board);

        if (king) { /// KING GENERATION FROM PRECALCULATED TABLES
            Position from_position = log2_64(king);
            bitboard attack_mask = processKing(from_position) & (~enemy_attack_mask);

            bitboard moves = attack_mask & empty;
            for (bitboard to_bit = takeBit(moves); to_bit; to_bit = takeBit(moves))
                out[counter++] = generateTurn<false, Figure::King>(from_position, log2_64(to_bit));

            bitboard attacks = attack_mask & enemies;
            for (bitboard to_bit = takeBit(attacks); to_bit; to_bit = takeBit(attacks))
                out[counter++] = generateTurn<true, Figure::King>(from_position, log2_64(to_bit));
        }

        return counter;
    }

    template<bool generate_moves, bool generate_attack = true>
    constexpr int generateFigures(int counter,
                                  bitboard empty,
                                  bitboard enemies,
                                  bitboard all,
                                  bitboard from_mask,
                                  bitboard to_move_mask,
                                  bitboard to_attack_mask)
    {
        Turn *out = storage;

        bitboard pawns = getPawns(board) & from_mask;
        bitboard knights = getKnights(board) & from_mask;
        bitboard bishops = getBishops(board) & from_mask;
        bitboard rooks = getRooks(board) & from_mask;
        bitboard queens = getQueens(board) & from_mask;

        { /// PAWNS GENERATION
            bitboard pawns_possible = pawnsShift<8>(pawns) & empty;
            bitboard pawns_possible_long = pawnsShift<8>(pawns_possible) & empty & to_move_mask
                                           & chooseMask(line_4, line_5);
            bitboard pawns_possible_left = pawnsShift<9>(pawns) & enemies & to_attack_mask
                                           & chooseMask(~row_h, ~row_a);
            bitboard pawns_possible_right = pawnsShift<7>(pawns) & enemies & to_attack_mask
                                            & chooseMask(~row_a, ~row_h);
            pawns_possible &= to_move_mask;

            empty &= to_move_mask;
            enemies &= to_attack_mask;

            if constexpr (generate_moves) {
                for (bitboard bit = takeBit(pawns_possible); bit; bit = takeBit(pawns_possible))
                    out[counter++] = generateTurnDelta<8, false, Figure::Pawn>(bit);

                for (bitboard bit = takeBit(pawns_possible_long); bit;
                     bit = takeBit(pawns_possible_long))
                    out[counter++] = generateTurnDelta<16, false, Figure::Pawn>(bit);
            }

            if constexpr (generate_attack) {
                for (bitboard bit = takeBit(pawns_possible_left); bit;
                     bit = takeBit(pawns_possible_left))
                    out[counter++] = generateTurnDelta<9, true, Figure::Pawn>(bit);

                for (bitboard bit = takeBit(pawns_possible_right); bit;
                     bit = takeBit(pawns_possible_right))
                    out[counter++] = generateTurnDelta<7, true, Figure::Pawn>(bit);

                if constexpr (flags & BitBoard::flags_el_passant) {
                    bitboard to_mask = positionToMask(
                        (board.m_turn.from().index() + board.m_turn.to().index()) / 2);
                    bitboard attack_cell = pawnsShift<-8>(to_mask);

                    if ((pawnsShift<9>(pawns) & to_mask) && (attack_cell & enemies))
                        out[counter++] = generateTurnDelta<9, true, Figure::Pawn>(to_mask);
                    if ((pawnsShift<7>(pawns) & to_mask) && (attack_cell & enemies))
                        out[counter++] = generateTurnDelta<7, true, Figure::Pawn>(to_mask);
                }
            }
        }

        { /// KNIGHTS GENERATION FROM PRECALCULATED TABLES
            for (bitboard bit = takeBit(knights); bit; bit = takeBit(knights)) {
                Position from_position = log2_64(bit);
                bitboard attack_mask = processKnight(from_position);

                if constexpr (generate_moves) {
                    bitboard moves = attack_mask & empty;
                    for (bitboard to_bit = takeBit(moves); to_bit; to_bit = takeBit(moves))
                        out[counter++] = generateTurn<false, Figure::Knight>(from_position,
                                                                             log2_64(to_bit));
                }

                if constexpr (generate_attack) {
                    bitboard attacks = attack_mask & enemies;
                    for (bitboard to_bit = takeBit(attacks); to_bit; to_bit = takeBit(attacks))
                        out[counter++] = generateTurn<true, Figure::Knight>(from_position,
                                                                            log2_64(to_bit));
                }
            }
        }

        { /// BISHOPS GENERATION WITH SOME MAGIC
            for (bitboard bit = takeBit(bishops); bit; bit = takeBit(bishops)) {
                Position from_position = log2_64(bit);
                bitboard attack_mask = processBishop(from_position, all);

                if constexpr (generate_moves) {
                    bitboard moves = attack_mask & empty;
                    for (bitboard to_bit = takeBit(moves); to_bit; to_bit = takeBit(moves))
                        out[counter++] = generateTurn<false, Figure::King>(from_position,
                                                                           log2_64(to_bit));
                }

                if constexpr (generate_attack) {
                    bitboard attacks = attack_mask & enemies;
                    for (bitboard to_bit = takeBit(attacks); to_bit; to_bit = takeBit(attacks))
                        out[counter++] = generateTurn<true, Figure::King>(from_position,
                                                                          log2_64(to_bit));
                }
            }
        }

        { /// ROOKS GENERATION WITH SOME MAGIC
            for (bitboard bit = takeBit(rooks); bit; bit = takeBit(rooks)) {
                Position from_position = log2_64(bit);
                bitboard attack_mask = processRook(from_position, all);

                if constexpr (generate_moves) {
                    bitboard moves = attack_mask & empty;
                    for (bitboard to_bit = takeBit(moves); to_bit; to_bit = takeBit(moves))
                        out[counter++] = generateTurn<false, Figure::King>(from_position,
                                                                           log2_64(to_bit));
                }

                if constexpr (generate_attack) {
                    bitboard attacks = attack_mask & enemies;
                    for (bitboard to_bit = takeBit(attacks); to_bit; to_bit = takeBit(attacks))
                        out[counter++] = generateTurn<true, Figure::King>(from_position,
                                                                          log2_64(to_bit));
                }
            }
        }

        { /// QUEENS GENERATION WITH DOUBLE MAGIC
            for (bitboard bit = takeBit(queens); bit; bit = takeBit(queens)) {
                Position from_position = log2_64(bit);
                bitboard attack_mask = processRook(from_position, all)
                                       | processBishop(from_position, all);

                if constexpr (generate_moves) {
                    bitboard moves = attack_mask & empty;
                    for (bitboard to_bit = takeBit(moves); to_bit; to_bit = takeBit(moves))
                        out[counter++] = generateTurn<false, Figure::King>(from_position,
                                                                           log2_64(to_bit));
                }

                if constexpr (generate_attack) {
                    bitboard attacks = attack_mask & enemies;
                    for (bitboard to_bit = takeBit(attacks); to_bit; to_bit = takeBit(attacks))
                        out[counter++] = generateTurn<true, Figure::King>(from_position,
                                                                          log2_64(to_bit));
                }
            }
        }

        return counter;
    }
};

BitBoard BitBoard::set(Position position, Figure figure) const
{
    BitBoard copy(*this);
    bitboard mask = positionToMask(position);

    switch (figure) {
    case Figure::Empty:
        copy.removeFigure(~mask);
        break;
    case Figure::WPawn:
        copy.removeFigure(~mask);
        copy.m_w_p |= mask;
        break;
    case Figure::WKnight:
        copy.removeFigure(~mask);
        copy.m_w_n |= mask;
        break;
    case Figure::WBishop:
        copy.removeFigure(~mask);
        copy.m_w_b |= mask;
        break;
    case Figure::WRook:
        copy.removeFigure(~mask);
        copy.m_w_r |= mask;
        break;
    case Figure::WQueen:
        copy.removeFigure(~mask);
        copy.m_w_q |= mask;
        break;
    case Figure::WKing:
        copy.removeFigure(~mask);
        copy.m_w_k |= mask;
        break;
    case Figure::BPawn:
        copy.removeFigure(~mask);
        copy.m_b_p |= mask;
        break;
    case Figure::BKnight:
        copy.removeFigure(~mask);
        copy.m_b_n |= mask;
        break;
    case Figure::BBishop:
        copy.removeFigure(~mask);
        copy.m_b_b |= mask;
        break;
    case Figure::BRook:
        copy.removeFigure(~mask);
        copy.m_b_r |= mask;
        break;
    case Figure::BQueen:
        copy.removeFigure(~mask);
        copy.m_b_q |= mask;
        break;
    case Figure::BKing:
        copy.removeFigure(~mask);
        copy.m_b_k |= mask;
        break;
    }

    return copy;
}

BitBoard BitBoard::setFlags(Flags flags) const
{
    BitBoard board(*this);
    board.m_flags = flags;
    return board;
}

BitBoard BitBoard::setTurn(Turn turn) const
{
    BitBoard board(*this);
    board.m_turn = turn;
    return board;
}

BitBoard BitBoard::swap(Position p1, Position p2) const
{
    if (p1 == p2)
        return *this;
    auto figure1 = get(p1);
    auto figure2 = get(p2);
    return set(p2, figure1).set(p1, figure2);
}

Figure BitBoard::get(Position position) const noexcept
{
    bitboard mask = positionToMask(position);
    if (mask & m_w_p)
        return Figure::WPawn;
    if (mask & m_w_n)
        return Figure::WKnight;
    if (mask & m_w_b)
        return Figure::WBishop;
    if (mask & m_w_r)
        return Figure::WRook;
    if (mask & m_w_q)
        return Figure::WQueen;
    if (mask & m_w_k)
        return Figure::WKing;
    if (mask & m_b_p)
        return Figure::BPawn;
    if (mask & m_b_n)
        return Figure::BKnight;
    if (mask & m_b_b)
        return Figure::BBishop;
    if (mask & m_b_r)
        return Figure::BRook;
    if (mask & m_b_q)
        return Figure::BQueen;
    if (mask & m_b_k)
        return Figure::BKing;
    return Figure::Empty;
}

BitBoard::Flags BitBoard::getFlags() const noexcept
{
    return m_flags;
}

Color BitBoard::getCurrentSide() const noexcept
{
    return (m_flags & Flags::flags_color) ? Color::Black : Color::White;
}

template<BitBoard::Flags flags>
constexpr int generateTemplate(const BitBoard &board, Turn *storage)
{
    return BitBoardHelper<flags>(board, storage).generate();
}

template<std::size_t... I>
constexpr auto generatePointerArray(std::index_sequence<I...>)
{
    using pointer = int (*)(const BitBoard &, Turn *storage);
    constexpr std::size_t N = sizeof...(I);
    return std::array<pointer, N>{{&generateTemplate<(BitBoard::Flags) I>...}};
}

constexpr int generate(const BitBoard &board, Turn *storage, BitBoard::Flags flags)
{
    constexpr auto pointers = generatePointerArray(
        std::make_index_sequence<BitBoard::flags_upper_bound>{});
    return pointers[flags](board, storage);
}

int BitBoard::getTurns(Color color, Turn *out) const
{
    auto flags = m_flags;
    if (color == Color::White)
        flags = (Flags) (flags & (~flags_color));
    else
        flags = (Flags) (flags | flags_color);

    return generate(*this, out, flags);
    //return BitBoardHelper<Flags::flags_default>(*this, out).generate();
}

BitBoard BitBoard::executeTurn(Color color, Turn turn)
{
    BitBoard copy(*this);

    if (color != Color::White)
        copy.m_flags = (Flags) (m_flags & ~flags_color);
    else
        copy.m_flags = (Flags) (m_flags | flags_color);

    bitboard from = positionToMask(turn.from());
    bitboard to = positionToMask(turn.to());
    if (color == Color::White) {
        copy.moveFromToWhite(from, to);
        copy.removeBlackFigure(~to);
    } else {
        copy.moveFromToBlack(from, to);
        copy.removeWhiteFigure(~to);
    }
    return copy;
}

BitBoard::BitBoardHash BitBoard::getHash() const
{
    return m_hash;
}

Turn BitBoard::getTurn() const
{
    return m_turn;
}
