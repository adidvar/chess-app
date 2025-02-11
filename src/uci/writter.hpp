#pragma once

#include <atomic>
#include <iostream>
#include <string>
#include <vector>

#include "chessai/ifeedback.hpp"
#include "chesscore/figures.hpp"

class Writter {
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

  static void option(const std::string& name, const std::string& type,
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

  static void bestMove(const std::string& move, const std::string& ponder = "") {
    std::cout << "bestmove " << move;
    if (!ponder.empty()) {
      std::cout << " ponder " << ponder;
    }
    std::cout << std::endl;
  }

  static void info(const std::string& key, const std::string& value) {
    std::cout << "info " << key << " " << value << std::endl;
  }

  static void infoPv(const std::vector<Turn>& moves) {
    std::cout << "info pv";
    for (const auto& move : moves) {
      std::cout << " " << move.toString();
    }
    std::cout << std::endl;
  }

  static void debug(const std::string& message) {
    std::cout << "info string debug: " << message << std::endl;
  }

  static void info(const std::string& message) {
    std::cout << "info string info: " << message << std::endl;
  }

  static void warning(const std::string& message) {
    std::cout << "info string warning: " << message << std::endl;
  }

  static void error(const std::string& message) {
    std::cout << "info string error: " << message << std::endl;
  }

  static void critical(const std::string& message) {
    std::cout << "info string critical: " << message << std::endl;
  }
};

class UCIFeedBack : public IFeedBack {
 public:
  virtual void sendBestMove(Turn bestMove) {
    std::cout << "bestmove " << bestMove.toString() << std::endl;
    ready_flag.store(true);
  }

  virtual void sendDepth(int depth) {
    std::cout << "info depth " << depth << std::endl;
  }

  virtual void sendScore(const std::string& score) {
    std::cout << "info score " << score << std::endl;
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

  virtual void sendPVLine(const std::vector<Turn>& pvLine) {
    std::cout << "info pv";
    for (const auto& move : pvLine) {
      std::cout << " " << move.toString();
    }
    std::cout << std::endl;
  }

  virtual void sendDebug(const std::string& message) {
    if (debug_enabled.load())
      std::cout << "info string debug: " << message << std::endl;
  }

  virtual void sendInfo(const std::string& message) {
    std::cout << "info string info: " << message << std::endl;
  }

  virtual void sendWarning(const std::string& message) {
    std::cout << "info string warning: " << message << std::endl;
  }

  virtual void sendError(const std::string& message) {
    std::cout << "info string error: " << message << std::endl;
  }

  virtual void sendCritical(const std::string& message) {
    std::cout << "info string critical: " << message << std::endl;
  }

  void setDebugMode(bool enabled) { debug_enabled.store(enabled); };
  bool getReadyFlag() const { return ready_flag.load(); };
  void resetReadyFlag() { ready_flag.store(false); };

  virtual ~UCIFeedBack() = default;

 private:
  std::atomic_bool debug_enabled = false;
  std::atomic_bool ready_flag = true;
};
