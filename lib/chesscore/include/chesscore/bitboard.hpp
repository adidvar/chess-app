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

    struct Flags
    {
        constexpr Flags() = default;
        constexpr Flags(bool w_ooo, bool w_oo, bool b_ooo, bool b_oo, bool el_passant, bool side)
            : white_ooo(w_ooo)
            , white_oo(w_oo)
            , black_ooo(b_ooo)
            , black_oo(b_oo)
            , last_turn_is_pawn(el_passant)
            , side(side)
        {}
        int8_t white_ooo : 1;
        int8_t white_oo : 1;
        int8_t black_ooo : 1;
        int8_t black_oo : 1;
        int8_t last_turn_is_pawn : 1;
        int8_t side : 1;
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

    void generateSubBoards(std::vector<BitBoard> &boards,
                           Color color,
                           bitboard from = 0xFFFFFFFFFFFFFFFF,
                           bitboard to = 0xFFFFFFFFFFFFFFFF) const;

    [[nodiscard]] BitBoard executeTurn(Turn turn);
    [[nodiscard]] bool testTurn(Turn turn) const;

    [[nodiscard]] BitBoardHash getHash() const;
    [[nodiscard]] Turn getTurn() const;

    bool operator==(const BitBoard &board) const = default;
    bool operator!=(const BitBoard &board) const = default;

private:
    void removeFigure(bitboard mask);
    void removeBlackFigure(bitboard mask);
    void removeWhiteFigure(bitboard mask);
    void copyWhites(const BitBoard &other);
    void copyBlacks(const BitBoard &other);
    bitboard getWhites() const;
    bitboard getBlacks() const;
    bitboard getAll() const;

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
    Flags m_flags{};

    template<typename Callback, int8_t color, bool rooking, bool el_passant>
    friend class BitBoardHelper;
};
