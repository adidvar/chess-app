#pragma once

#include <chesscore/figures.hpp>
#include <iostream>
#include <string>
#include <vector>

class IFeedBack {
 public:
  virtual void sendInfo(const std::string& info) {
    std::cout << "info string " << info << std::endl;
  }

  virtual void sendBestMove(Turn bestMove) {
    std::cout << "bestmove " << bestMove.toString() << std::endl;
  }

  virtual void sendDepth(int depth) {
    std::cout << "info depth " << depth << std::endl;
  }

  virtual void sendScore(float score) {
    std::cout << "info score cp " << score << std::endl;
  }

  virtual void sendTurn(Turn turn) {
    std::cout << "info turn " << turn.toString() << std::endl;
  }

  virtual void sendNodesSearched(size_t nodes) {
    std::cout << "info nodes " << nodes << std::endl;
  }

  virtual void sendTimeElapsed(int milliseconds) {
    std::cout << "info time " << milliseconds << std::endl;
  }

  virtual void sendPVLine(const std::vector<std::string>& pvLine) {
    std::cout << "info pv";
    for (const auto& move : pvLine) {
      std::cout << " " << move;
    }
    std::cout << std::endl;
  }

  virtual ~IFeedBack() = default;
};
