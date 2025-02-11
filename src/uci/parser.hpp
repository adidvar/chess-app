#pragma once

#include <sstream>
#include <vector>

#include "chesscore/fen.hpp"
#include "reader.hpp"
#include "writter.hpp"

class Parser : public Reader, public Writter {
 public:
  void loop() {
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
        sendWarning("unknown command '" + command + "'");
      }
    }
  }

 private:
  void parseSetOption(std::istringstream& stream) {
    std::string name, nameValue, value, valueValue;
    stream >> name >> nameValue;
    if (name != "name") {
      sendWarning("invalid setoption command format");
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
    try {
      std::string token;
      stream >> token;

      BitBoard fen;
      if (token == "startpos") {
        fen = {"startpos"};
        stream >> token;
      } else if (token == "fen") {
        size_t index = stream.tellg();
        boardFromFen(stream.str(), fen, index);
        stream.ignore(index - stream.tellg());
        stream >> token;
      } else {
        sendWarning("invalid position command format");
        return;
      }

      std::vector<Turn> moves;
      while (stream >> token) {
        moves.push_back(Turn(token));
      }
      position(fen, moves);
    } catch (const FenParsingError& error) {
      sendWarning(error.what());
    }
  }

  void parseGo(std::istringstream& stream) {
    std::string params;
    std::getline(stream, params);
    go(params);
  }
};
