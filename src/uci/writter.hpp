#pragma once

#include <iostream>
#include <string>
#include <vector>

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

  static void infoPv(const std::vector<std::string>& moves) {
    std::cout << "info pv";
    for (const auto& move : moves) {
      std::cout << " " << move;
    }
    std::cout << std::endl;
  }

  static void string(const std::string& message) {
    std::cout << "info string " << message << std::endl;
  }

  static void debugOutput(const std::string& message) {
    std::cout << "debug " << message << std::endl;
  }
};
