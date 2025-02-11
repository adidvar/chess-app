#pragma once

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
  virtual void position(const BitBoard& fen,
                        const std::vector<Turn>& moves) = 0;
  virtual void go(const std::string& parameters) = 0;
  virtual void stop() = 0;
  virtual void ponderHit() = 0;
  virtual void quit() = 0;

  virtual ~Reader() = default;
};
