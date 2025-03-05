#include "evaluator.hpp"

#include <array>
#include <chesscore/bitutils.hpp>
#include <cmath>

struct S {
  constexpr S() : mg(0), eg(0) {}
  constexpr S(int m) : mg(m), eg(m) {}
  constexpr S(int m, int e) : mg(m), eg(e) {}

  int mg;
  int eg;

  constexpr int operator()() const { return mg; }
};

float EvaluatedBitBoard::toCentiPawns(int value) {
  return 20.0 / (1.0 + pow(3.0, -value / 1260.0));
}

constexpr auto p_e = S(0, 0);
constexpr auto p_p = S(126, 208);
constexpr auto p_n = S(781, 854);
constexpr auto p_b = S(825, 915);
constexpr auto p_r = S(1276, 1380);
constexpr auto p_q = S(2538, 2682);

constexpr S prices[]{p_e, p_p, p_n, p_b, p_r, p_q};

EvaluatedBitBoard::ScoreType EvaluatedBitBoard::evaluateSEE(Turn turn) const {
  auto from_figure = get(turn.from());
  auto to_figure = get(turn.to());
  return prices[to_figure]() - prices[from_figure]();
}

int EvaluatedBitBoard::getMaterial() const {
  int value = 0;

  value += p_p() * (popCount(m_w_p) - popCount(m_b_p));
  value += p_n() * (popCount(m_w_n) - popCount(m_b_n));
  value += p_b() * (popCount(m_w_b) - popCount(m_b_b));
  value += p_r() * (popCount(m_w_r) - popCount(m_b_r));
  value += p_q() * (popCount(m_w_q) - popCount(m_b_q));

  return value;
}

EvaluatedBitBoard::ScoreType EvaluatedBitBoard::evaluate() const {
  return getMaterial() + getTables();
}

const std::array<int, 64> p_p_p = {
    0,  0,  0,  0,   0,   0,  0,  0,  50, 50, 50,  50, 50, 50,  50, 50,
    10, 10, 20, 30,  30,  20, 10, 10, 5,  5,  10,  25, 25, 10,  5,  5,
    0,  0,  0,  20,  20,  0,  0,  0,  5,  -5, -10, 0,  0,  -10, -5, 5,
    5,  10, 10, -20, -20, 10, 10, 5,  0,  0,  0,   0,  0,  0,   0,  0};

const std::array<int, 64> p_p_n = {
    -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   0,   0,
    0,   -20, -40, -30, 0,   10,  15,  15,  10,  0,   -30, -30, 5,
    15,  20,  20,  15,  5,   -30, -30, 0,   15,  20,  20,  15,  0,
    -30, -30, 5,   10,  15,  15,  10,  5,   -30, -40, -20, 0,   5,
    5,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50,
};
const std::array<int, 64> p_p_b = {
    -20, -10, -10, -10, -10, -10, -10, -20, -10, 0,   0,   0,   0,
    0,   0,   -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 5,
    5,   10,  10,  5,   5,   -10, -10, 0,   10,  10,  10,  10,  0,
    -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 5,   0,   0,
    0,   0,   5,   -10, -20, -10, -10, -10, -10, -10, -10, -20,
};

const std::array<int, 64> p_p_r = {
    0,  0, 0, 0, 0, 0, 0, 0,  5,  10, 10, 10, 10, 10, 10, 5,
    -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
    -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
    -5, 0, 0, 0, 0, 0, 0, -5, 0,  0,  0,  5,  5,  0,  0,  0};

const std::array<int, 64> p_p_q = {
    -20, -10, -10, -5, -5, -10, -10, -20, -10, 0,   0,   0,  0,  0,   0,   -10,
    -10, 0,   5,   5,  5,  5,   0,   -10, -5,  0,   5,   5,  5,  5,   0,   -5,
    0,   0,   5,   5,  5,  5,   0,   -5,  -10, 5,   5,   5,  5,  5,   0,   -10,
    -10, 0,   5,   0,  0,  0,   0,   -10, -20, -10, -10, -5, -5, -10, -10, -20};

const std::array<int, 64> p_p_k = {
    -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50,
    -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40,
    -40, -50, -50, -40, -40, -30, -20, -30, -30, -40, -40, -30, -30,
    -20, -10, -20, -20, -20, -20, -20, -20, -10, 20,  20,  0,   0,
    0,   0,   20,  20,  20,  30,  10,  0,   0,   10,  30,  20};

int EvaluatedBitBoard::getTables() const {
  int value = 0;

  auto lambdaw = [&value](const std::array<int, 64> &table, bitboard board) {
    for (bitboard bit = takeBit(board); bit; bit = takeBit(board))
      value += table[log2_64(bit)];
  };

  auto lambdab = [&value](const std::array<int, 64> &table, bitboard board) {
    for (bitboard bit = takeBit(board); bit; bit = takeBit(board))
      value -= table[Position(log2_64(bit)).rotate().index()];
  };

  lambdaw(p_p_p, m_w_p);
  lambdaw(p_p_n, m_w_n);
  lambdaw(p_p_b, m_w_b);
  lambdaw(p_p_r, m_w_r);
  lambdaw(p_p_q, m_w_q);
  lambdaw(p_p_k, m_w_k);

  lambdab(p_p_p, m_b_p);
  lambdab(p_p_n, m_b_n);
  lambdab(p_p_b, m_b_b);
  lambdab(p_p_r, m_b_r);
  lambdab(p_p_q, m_b_q);
  lambdab(p_p_k, m_b_k);

  return value;
}
