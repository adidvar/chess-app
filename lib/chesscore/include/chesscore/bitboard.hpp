#pragma once

#include <vector>

#include "figures.hpp"
#include "position.hpp"
#include "turn.hpp"

class BitBoard
{
public:
    using bitboard = uint64_t;
    using BitBoardHash = uint64_t;

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

    [[nodiscard]] std::vector<BitBoard> generateSubBoards(Color color,
                                                          bitboard from = 0xFFFFFFFFFFFFFFFF,
                                                          bitboard to = 0xFFFFFFFFFFFFFFFF) const;

    int getSubBoardsCounter(Color color,
                            bitboard from = 0xFFFFFFFFFFFFFFFF,
                            bitboard to = 0xFFFFFFFFFFFFFFFF) const;

    [[nodiscard]] BitBoard executeTurn(Turn turn);
    [[nodiscard]] bool testTurn(Turn turn) const;

    [[nodiscard]] BitBoardHash getHash() const;
    [[nodiscard]] Turn getTurn() const;

    bool operator==(const BitBoard &board) const = default;
    bool operator!=(const BitBoard &board) const = default;

private:
    constexpr void removeFigure(bitboard mask);
    constexpr void removeBlackFigure(bitboard mask);
    constexpr void removeWhiteFigure(bitboard mask);
    constexpr void copyWhites(const BitBoard &other);
    constexpr void copyBlacks(const BitBoard &other);
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

    template<typename Callback, BitBoard::Flags flags>
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

constexpr BitBoard::bitboard BitBoard::getWhites() const
{
    return m_w_p | m_w_n | m_w_b | m_w_r | m_w_q | m_w_k;
}

constexpr BitBoard::bitboard BitBoard::getBlacks() const
{
    return m_b_p | m_b_n | m_b_b | m_b_r | m_b_q | m_b_k;
}

constexpr BitBoard::bitboard BitBoard::getAll() const
{
    return getWhites() | getBlacks();
}