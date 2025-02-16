#pragma once

#include <chesscore/figures.hpp>
#include <string>
#include <vector>

class IFeedBack {
 public:
  virtual void setDepth(int depth) {}
  virtual void setSelDepth(int depth) {}
  virtual void setTimeElapsed(int milliseconds) {}
  virtual void setNodesSearched(size_t nodes) {}
  virtual void setNPS(size_t nodes) {}
  virtual void setScore(const std::string& score) {}
  virtual void setPVLine(const std::vector<Turn>& pvLine) {}
  virtual void flush() {};
  virtual void flushMate() {};  // mate information flush

  virtual void sendBestMove(Turn bestMove) {}

  virtual void sendDebug(const std::string& message) {}
  virtual void sendInfo(const std::string& message) {}
  virtual void sendWarning(const std::string& message) {}
  virtual void sendError(const std::string& message) {}
  virtual void sendCritical(const std::string& message) {}

  virtual ~IFeedBack() = default;
};
