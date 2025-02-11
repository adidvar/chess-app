#pragma once

#include <chesscore/figures.hpp>
#include <string>
#include <vector>

class IFeedBack {
 public:
  virtual void sendBestMove(Turn bestMove) {}

  virtual void sendDepth(int depth) {
  }

  virtual void sendScore(const std::string& score) {}

  virtual void sendTurn(Turn turn) {
  }

  virtual void sendNodesSearched(size_t nodes) {
  }

  virtual void sendTimeElapsed(int milliseconds) {
  }

  virtual void sendPVLine(const std::vector<Turn>& pvLine) {}

  virtual void sendDebug(const std::string& message) {}

  virtual void sendInfo(const std::string& message) {}

  virtual void sendWarning(const std::string& message) {}

  virtual void sendError(const std::string& message) {}

  virtual void sendCritical(const std::string& message) {}

  virtual ~IFeedBack() = default;
};
