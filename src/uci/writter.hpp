#pragma once

#include <atomic>
#include <format>
#include <iostream>
#include <string>
#include <vector>

#include "chessai/ifeedback.hpp"
#include "chesscore/figures.hpp"

class Writter : public IFeedBack {
 public:
  static void idName(const std::string& name) {
    std::cout << "id name " << name << std::endl;
  }

  static void idAuthor(const std::string& author) {
    std::cout << "id author " << author << std::endl;
  }

  static void uciOk() {
    std::cout << "uciok" << std::endl;
  }

  static void readyOk() {
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

  virtual void setDepth(int depth) override {
    this->depth = depth;
  }
  virtual void setSelDepth(int depth) override {
    this->seldepth = depth;
  }
  virtual void setTimeElapsed(int milliseconds) override {
    this->time = milliseconds;
  }
  virtual void setNodesSearched(size_t nodes) override {
    this->nodes = nodes;
  }
  virtual void setNPS(size_t nodes) override {
    this->nps = nodes;
  }
  virtual void setScore(const std::string& score) override {
    this->score = score;
  }
  virtual void setPVLine(const std::vector<Turn>& pvLine) override {
    this->pv = pvLine;
  }
  virtual void flush() override {
    std::cout << std::format(
        "info depth {} seldepth {} score {} nodes {} nps {} time {} pv ", depth,
        seldepth, score, nodes, nps, time);
    for (auto turn : pv) std::cout << turn.toString() << " ";
    std::cout << std::endl;
  };

  virtual void sendDebug(const std::string& message) override {
    if (debug_enabled.load())
      std::cout << "info string " << message << std::endl;
  }

  virtual void sendInfo(const std::string& message) override {
    std::cout << "info string " << message << std::endl;
  }

  virtual void sendWarning(const std::string& message) override {
    std::cout << "info string warning " << message << std::endl;
  }

  virtual void sendError(const std::string& message) override {
    debug_enabled.store(true);
    std::cout << "info string error " << message << std::endl;
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
  // buffer
  int depth;
  int seldepth;
  long time;
  size_t nodes;
  int nps;
  std::string score;
  std::vector<Turn> pv;

  // control
  std::atomic_bool debug_enabled = false;
  std::atomic_bool ready_flag = true;
};

void sendCritical(const std::string& message) {
  std::cerr << "critical error: " << message << std::endl;
  exit(-1);
}
