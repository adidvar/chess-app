#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

#include "reader.hpp"

class Parser : public Reader {
 public:
  void parse() {
    std::string line;
    while (std::getline(std::cin, line)) {
      std::istringstream stream(line);
      std::string command;
      stream >> command;

      if (command == "uci") {
        uci();
      } else if (command == "debug") {
        std::string state;
        stream >> state;
        debug(state == "on");
      } else if (command == "isready") {
        isReady();
      } else if (command == "setoption") {
        parseSetOption(stream);
      } else if (command == "ucinewgame") {
        uciNewGame();
      } else if (command == "position") {
        parsePosition(stream);
      } else if (command == "go") {
        parseGo(stream);
      } else if (command == "stop") {
        stop();
      } else if (command == "ponderhit") {
        ponderHit();
      } else if (command == "quit") {
        quit();
        break;  
      } else {
        std::cerr << "Unknown command: " << command << std::endl;
      }
    }
  }

 private:
  void parseSetOption(std::istringstream& stream) {
    std::string name, nameValue, value, valueValue;
    stream >> name >> nameValue;
    if (name != "name") {
      std::cerr << "Invalid setoption command format" << std::endl;
      return;
    }

    std::string remaining;
    std::getline(stream, remaining);
    auto pos = remaining.find("value ");
    if (pos != std::string::npos) {
      valueValue = remaining.substr(pos + 6);
      remaining = remaining.substr(0, pos);
    }

    setOption(nameValue, valueValue);
  }

  void parsePosition(std::istringstream& stream) {
    std::string token, fen;
    stream >> token;
    if (token == "startpos") {
      fen = "startpos";
      stream >> token; 
    } else if (token == "fen") {
      std::getline(stream, fen, ' ');
    } else {
      std::cerr << "Invalid position command format" << std::endl;
      return;
    }

    std::vector<std::string> moves;
    while (stream >> token) {
      moves.push_back(token);
    }
    position(fen, moves);
  }

  void parseGo(std::istringstream& stream) {
    std::string params;
    std::getline(stream, params);
    go(params);
  }
};
