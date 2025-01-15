#pragma once

#include <vector>

#include "figures.hpp"

using bitboard = uint64_t;
using BitBoardHash = uint64_t;

class BitBoard
{
public:

    enum Flags {
        flags_default = 0,
        flags_color = 1,
        flags_el_passant = 2,
        flags_white_oo = 4,
        flags_white_ooo = 8,
        flags_black_oo = 16,
        flags_black_ooo = 32,
        flags_upper_bound = 64 //upper bound for branch-less generator
    };

    BitBoard();
    BitBoard(std::string_view fen_line);
    [[nodiscard]] std::string fen() const;

    [[nodiscard]] BitBoard set(Position position, Figure figure) const;
    [[nodiscard]] BitBoard setFlags(Flags flags) const;
    [[nodiscard]] BitBoard setTurn(Turn turn) const;

    [[nodiscard]] BitBoard swap(Position p1, Position p2) const;

    [[nodiscard]] Figure get(Position position) const noexcept;
    [[nodiscard]] Flags getFlags() const noexcept;
    [[nodiscard]] Color getCurrentSide() const noexcept;

    int getTurns(Color color, Turn *out) const;

    [[nodiscard]] BitBoard executeTurn(Color color, Turn turn) const;

    [[nodiscard]] BitBoardHash getHash() const;
    [[nodiscard]] Turn getTurn() const;

    bool operator==(const BitBoard &board) const = default;
    bool operator!=(const BitBoard &board) const = default;

protected:
    constexpr void removeFigure(bitboard mask);
    constexpr void removeBlackFigure(bitboard mask);
    constexpr void removeWhiteFigure(bitboard mask);
    constexpr void copyWhites(const BitBoard &other);
    constexpr void copyBlacks(const BitBoard &other);
    constexpr void moveFromToWhite(bitboard from, bitboard to);
    constexpr void moveFromToBlack(bitboard from, bitboard to);
    constexpr void promoteWhiteFigure(bitboard position, Figure figure);
    constexpr void promoteBlackFigure(bitboard position, Figure figure);
    constexpr bitboard getWhites() const;
    constexpr bitboard getBlacks() const;
    constexpr bitboard getAll() const;

    static const char *const kStartPosition;
    static const BitBoard kStartBitBoard;

    // bitboards white
    bitboard m_w_p = 0;
    bitboard m_w_n = 0;
    bitboard m_w_b = 0;
    bitboard m_w_r = 0;
    bitboard m_w_q = 0;
    bitboard m_w_k = 0;
    // bitboards blackk
    bitboard m_b_p = 0;
    bitboard m_b_n = 0;
    bitboard m_b_b = 0;
    bitboard m_b_r = 0;
    bitboard m_b_q = 0;
    bitboard m_b_k = 0;
    // state init
    BitBoardHash m_hash = 0;
    Turn m_turn{};
    // additional state
    Flags m_flags = flags_default;

    template<BitBoard::Flags flags>
    friend class BitBoardHelper;
};

constexpr void BitBoard::removeFigure(bitboard mask)
{
    removeBlackFigure(mask);
    removeWhiteFigure(mask);
}

constexpr void BitBoard::removeBlackFigure(bitboard mask)
{
    m_b_p &= mask;
    m_b_n &= mask;
    m_b_b &= mask;
    m_b_r &= mask;
    m_b_q &= mask;
    m_b_k &= mask;
}

constexpr void BitBoard::removeWhiteFigure(bitboard mask)
{
    m_w_p &= mask;
    m_w_n &= mask;
    m_w_b &= mask;
    m_w_r &= mask;
    m_w_q &= mask;
    m_w_k &= mask;
}

constexpr void BitBoard::copyWhites(const BitBoard &other)
{
    m_w_p = other.m_w_p;
    m_w_n = other.m_w_n;
    m_w_b = other.m_w_b;
    m_w_r = other.m_w_r;
    m_w_q = other.m_w_q;
    m_w_k = other.m_w_k;
}

constexpr void BitBoard::copyBlacks(const BitBoard &other)
{
    m_b_p = other.m_b_p;
    m_b_n = other.m_b_n;
    m_b_b = other.m_b_b;
    m_b_r = other.m_b_r;
    m_b_q = other.m_b_q;
    m_b_k = other.m_b_k;
}

constexpr bitboard BitBoard::getWhites() const
{
    return m_w_p | m_w_n | m_w_b | m_w_r | m_w_q | m_w_k;
}

constexpr bitboard BitBoard::getBlacks() const
{
    return m_b_p | m_b_n | m_b_b | m_b_r | m_b_q | m_b_k;
}

constexpr bitboard BitBoard::getAll() const
{
    return getWhites() | getBlacks();
}

constexpr void BitBoard::moveFromToWhite(bitboard from, bitboard to)
{
    m_w_p |= ((m_w_p & from) > 0) * to;
    m_w_p &= ~from;

    m_w_n |= ((m_w_n & from) > 0) * to;
    m_w_n &= ~from;

    m_w_b |= ((m_w_b & from) > 0) * to;
    m_w_b &= ~from;

    m_w_r |= ((m_w_r & from) > 0) * to;
    m_w_r &= ~from;

    m_w_k |= ((m_w_k & from) > 0) * to;
    m_w_k &= ~from;

    m_w_q |= ((m_w_q & from) > 0) * to;
    m_w_q &= ~from;
}

constexpr void BitBoard::moveFromToBlack(bitboard from, bitboard to)
{
    m_b_p |= ((m_b_p & from) > 0) * to;
    m_b_p &= ~from;

    m_b_n |= ((m_b_n & from) > 0) * to;
    m_b_n &= ~from;

    m_b_b |= ((m_b_b & from) > 0) * to;
    m_b_b &= ~from;

    m_b_r |= ((m_b_r & from) > 0) * to;
    m_b_r &= ~from;

    m_b_k |= ((m_b_k & from) > 0) * to;
    m_b_k &= ~from;

    m_b_q |= ((m_b_q & from) > 0) * to;
    m_b_q &= ~from;
}

constexpr void BitBoard::promoteWhiteFigure(bitboard position, Figure figure)
{
    switch (figure) {
    case Figure::Knight:
        m_w_n |= position;
        break;
    case Figure::Bishop:
        m_w_b |= position;
        break;
    case Figure::Rook:
        m_w_r |= position;
        break;
    case Figure::Queen:
        m_w_q |= position;
        break;
    }
}
constexpr void BitBoard::promoteBlackFigure(bitboard position, Figure figure)
{
    switch (figure) {
    case Figure::Knight:
        m_b_n |= position;
        break;
    case Figure::Bishop:
        m_b_b |= position;
        break;
    case Figure::Rook:
        m_b_r |= position;
        break;
    case Figure::Queen:
        m_b_q |= position;
        break;
    }
}
