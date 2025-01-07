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
    template<uint8_t shift>
    constexpr bitboard pawnsShift(bitboard in)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return in >> shift;
        else
            return in << shift;
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

    template<int8_t delta, bool attack, int8_t figure>
    constexpr Turn generateTurnFigure(bitboard to)
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
    constexpr static int getIntReversed(int value)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return value;
        else
            return -value;
    }
    constexpr bitboard &getKing(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
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
    constexpr bitboard &getRooks(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
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
    constexpr void removeEnemies(BitBoard &board, bitboard mask)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.removeBlackFigure(mask);
        else
            return board.removeWhiteFigure(mask);
    }
    constexpr void restoreEnemies(BitBoard &board, const BitBoard &from)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.copyBlacks(from);
        else
            return board.copyWhites(from);
    }

    constexpr int generate()
    {
        int counter = 0;
        Turn *out = storage;

        bitboard allies = getAlies();
        bitboard enemies = getEnemies();
        bitboard all = allies | enemies;
        bitboard empty = ~all;

        { /// PAWNS GENEARTION
            bitboard pawns = getPawns(board);

            bitboard possible = pawnsShift<8>(pawns) & empty;
            bitboard possible_long = pawnsShift<8>(possible) & empty;
            bitboard possible_left = pawnsShift<9>(pawns) & enemies;
            bitboard possible_right = pawnsShift<7>(pawns) & enemies;

            for (bitboard bit = takeBit(possible); bit; bit = takeBit(possible))
                out[counter++] = generateTurnDelta<8, false, Figure::Pawn>(bit);

            for (bitboard bit = takeBit(possible_long); bit; bit = takeBit(possible_long))
                out[counter++] = generateTurnDelta<16, false, Figure::Pawn>(bit);

            for (bitboard bit = takeBit(possible_left); bit; bit = takeBit(possible_left))
                out[counter++] = generateTurnDelta<9, true, Figure::Pawn>(bit);

            for (bitboard bit = takeBit(possible_right); bit; bit = takeBit(possible_right))
                out[counter++] = generateTurnDelta<7, true, Figure::Pawn>(bit);

            if constexpr (flags & BitBoard::flags_el_passant) {
                bitboard to_mask = positionToMask(
                    (board.m_turn.from().index() + board.m_turn.to().index()) / 2);
                if (pawnsShift<9>(pawns) & to_mask)
                    out[counter++] = generateTurnDelta<9, true, Figure::Pawn>(to_mask);
                if (pawnsShift<7>(pawns) & to_mask)
                    out[counter++] = generateTurnDelta<7, true, Figure::Pawn>(to_mask);
            }
        }

        { /// KNIGHTS GENERATION FROM PRECALCULATED TABLES
            bitboard knights = getKnights(board);
            for (bitboard bit = takeBit(knights); bit; bit = takeBit(knights)) {
                Position from_position = log2_64(bit);
                bitboard attack_mask = processKnight(from_position);

                bitboard moves = attack_mask & empty;
                for (bitboard to_bit = takeBit(moves); to_bit; to_bit = takeBit(moves))
                    out[counter++] = generateTurn<false, Figure::Knight>(from_position,
                                                                         log2_64(to_bit));

                bitboard attacks = attack_mask & enemies;
                for (bitboard to_bit = takeBit(attacks); to_bit; to_bit = takeBit(attacks))
                    out[counter++] = generateTurn<true, Figure::Knight>(from_position,
                                                                        log2_64(to_bit));
            }
        }

        { /// KING GENERATION FROM PRECALCULATED TABLES
            bitboard king = getKing(board);
            for (bitboard bit = takeBit(king); bit; bit = takeBit(king)) {
                Position from_position = log2_64(bit);
                bitboard attack_mask = processKing(from_position);

                bitboard moves = attack_mask & empty;
                for (bitboard to_bit = takeBit(moves); to_bit; to_bit = takeBit(moves))
                    out[counter++] = generateTurn<false, Figure::King>(from_position,
                                                                       log2_64(to_bit));

                bitboard attacks = attack_mask & enemies;
                for (bitboard to_bit = takeBit(attacks); to_bit; to_bit = takeBit(attacks))
                    out[counter++] = generateTurn<true, Figure::King>(from_position,
                                                                      log2_64(to_bit));
            }
        }

        { /// BISHOPS GENERATION WITH SOME MAGIC
            bitboard bishops = getBishops(board);
            for (bitboard bit = takeBit(bishops); bit; bit = takeBit(bishops)) {
                Position from_position = log2_64(bit);
                bitboard attack_mask = processBishop(from_position, all);

                bitboard moves = attack_mask & empty;
                for (bitboard to_bit = takeBit(moves); to_bit; to_bit = takeBit(moves))
                    out[counter++] = generateTurn<false, Figure::King>(from_position,
                                                                       log2_64(to_bit));

                bitboard attacks = attack_mask & enemies;
                for (bitboard to_bit = takeBit(attacks); to_bit; to_bit = takeBit(attacks))
                    out[counter++] = generateTurn<true, Figure::King>(from_position,
                                                                      log2_64(to_bit));
            }
        }

        { /// ROOKS GENERATION WITH SOME MAGIC
            bitboard rooks = getRooks(board);
            for (bitboard bit = takeBit(rooks); bit; bit = takeBit(rooks)) {
                Position from_position = log2_64(bit);
                bitboard attack_mask = processRook(from_position, all);

                bitboard moves = attack_mask & empty;
                for (bitboard to_bit = takeBit(moves); to_bit; to_bit = takeBit(moves))
                    out[counter++] = generateTurn<false, Figure::King>(from_position,
                                                                       log2_64(to_bit));

                bitboard attacks = attack_mask & enemies;
                for (bitboard to_bit = takeBit(attacks); to_bit; to_bit = takeBit(attacks))
                    out[counter++] = generateTurn<true, Figure::King>(from_position,
                                                                      log2_64(to_bit));
            }
        }

        { /// QUEENS GENERATION WITH DOUBLE MAGIC
            bitboard queens = getQueens(board);
            for (bitboard bit = takeBit(queens); bit; bit = takeBit(queens)) {
                Position from_position = log2_64(bit);
                bitboard attack_mask = processRook(from_position, all)
                                       | processBishop(from_position, all);

                bitboard moves = attack_mask & empty;
                for (bitboard to_bit = takeBit(moves); to_bit; to_bit = takeBit(moves))
                    out[counter++] = generateTurn<false, Figure::King>(from_position,
                                                                       log2_64(to_bit));

                bitboard attacks = attack_mask & enemies;
                for (bitboard to_bit = takeBit(attacks); to_bit; to_bit = takeBit(attacks))
                    out[counter++] = generateTurn<true, Figure::King>(from_position,
                                                                      log2_64(to_bit));
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

    if (color == Color::Black) {
        copy.m_flags = (Flags) (copy.m_flags & (~Flags::flags_color));
    } else {
        copy.m_flags = (Flags) (copy.m_flags | Flags::flags_color);
    }

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
