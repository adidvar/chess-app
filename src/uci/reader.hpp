#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "chesscore/bitboard.hpp"
#include "chesscore/figures.hpp"

class Reader {
 public:
  virtual void uci() = 0;
  virtual void debug(bool enable) = 0;
  virtual void isReady() = 0;
  virtual void setOption(const std::string& name, const std::string& value) = 0;
  virtual void uciNewGame() = 0;
  virtual void position(const BitBoard& fen) = 0;
  virtual void ponderHit() {};

  // search types
  virtual void go() = 0;
  virtual void goPerft(int depth) = 0;
  virtual void goMate(int depth) = 0;

  // go parameters
  virtual void goPonder() {};
  virtual void goTimeControl(int wsec, int bsec, int winc, int binc) = 0;
  virtual void goTime(int msec) = 0;
  virtual void goDepth(int depth) = 0;
  virtual void goNodes(long nodes) = 0;
  virtual void goMoves(std::vector<Turn> turns) = 0;
  virtual void goInfinite() = 0;
  virtual void goMovesToGo(int moves) = 0;

  virtual void stop() = 0;
  virtual void quit() = 0;

  virtual ~Reader() = default;
};
