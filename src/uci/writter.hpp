#pragma once

#include <atomic>
#include <iostream>
#include <string>
#include <vector>

#include "chessai/ifeedback.hpp"
#include "chesscore/figures.hpp"

class Writter : public IFeedBack {
 public:
  void idName(const std::string& name) {
    std::cout << "id name " << name << std::endl;
  }

  void idAuthor(const std::string& author) {
    std::cout << "id author " << author << std::endl;
  }

  void uciOk() {
    std::cout << "uciok" << std::endl;
  }

  void readyOk() {
    std::cout << "readyok" << std::endl;
  }

  void option(const std::string& name, const std::string& type,
              const std::string& defaultValue = "", int min = 0, int max = 0) {
    std::cout << "option name " << name << " type " << type;
    if (!defaultValue.empty()) {
      std::cout << " default " << defaultValue;
    }
    if (type == "spin" || type == "check") {
      std::cout << " min " << min << " max " << max;
    }
    std::cout << std::endl;
  }

  virtual void sendBestMove(Turn bestMove) override {
    std::cout << "bestmove " << bestMove.toString() << std::endl;
    ready_flag.store(true);
  }

  virtual void sendDepth(int depth) override {
    std::cout << "info depth " << depth << std::endl;
  }

  virtual void sendScore(const std::string& score) override {
    std::cout << "info score " << score << std::endl;
  }

  virtual void sendTurn(Turn turn) override {
    std::cout << "info turn " << turn.toString() << std::endl;
  }

  virtual void sendNodesSearched(size_t nodes) override {
    std::cout << "info nodes " << nodes << std::endl;
  }

  virtual void sendTimeElapsed(int milliseconds) override {
    std::cout << "info time " << milliseconds << std::endl;
  }

  virtual void sendPVLine(const std::vector<Turn>& pvLine) override {
    std::cout << "info pv";
    for (const auto& move : pvLine) {
      std::cout << " " << move.toString();
    }
    std::cout << std::endl;
  }

  virtual void sendDebug(const std::string& message) override {
    if (debug_enabled.load())
      std::cout << "info string debug: " << message << std::endl;
  }

  virtual void sendInfo(const std::string& message) override {
    std::cout << "info string info: " << message << std::endl;
  }

  virtual void sendWarning(const std::string& message) override {
    std::cout << "info string warning: " << message << std::endl;
  }

  virtual void sendError(const std::string& message) override {
    std::cout << "info string error: " << message << std::endl;
  }

  void setDebugMode(bool enabled) {
    debug_enabled.store(enabled);
  };
  bool getReadyFlag() const {
    return ready_flag.load();
  };
  void resetReadyFlag() {
    ready_flag.store(false);
  };

  virtual ~Writter() = default;

 private:
  std::atomic_bool debug_enabled = false;
  std::atomic_bool ready_flag = true;
};

void sendCritical(const std::string& message) {
  std::cerr << "critical error:  " << message << std::endl;
  exit(-1);
}
