#pragma once

#include <optional>
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
      } else if (command == "perft") {
        parsePerft(stream);
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
        auto turn = Turn(token);
        if (!turn.isValid()) {
          sendWarning("invalid turn format");
          return;
        }
        if (!fen.testTurn(turn)) {
          sendWarning("invalid turn");
          return;
        }
        fen = fen.executeTurn(fen.side(), turn);
      }
      position(fen);
    } catch (const FenError& error) {
      sendWarning(error.what());
    }
  }

  void parseGo(std::istringstream& stream) {
    std::string token;
    std::vector<Turn> move_list;
    bool ponder_flag = false, infinite_flag = false, perft_flag = false;
    std::optional<int> white_time, black_time, white_inc, black_inc;
    std::optional<int> moves_to_go_val, depth_val, nodes_val, mate_val,
        move_time_val, perft_depth;

    while (stream >> token) {
      if (token == "ponder") {
        ponder_flag = true;
      } else if (token == "infinite") {
        infinite_flag = true;
      } else if (token == "perft") {
        perft_flag = true;
        if (!(stream >> perft_depth.emplace())) {
          sendWarning("Missing value for perft depth");
          return;
        }
      } else if (token == "moves") {
        while (stream >> token) {
          auto turn = Turn(token);
          if (!turn.isValid()) break;
          move_list.push_back(turn);
        }
        break;
      } else {
        int value;
        if (!(stream >> value)) {
          sendWarning("Missing or invalid value for parameter: " + token);
          return;
        }
        if (token == "wtime") {
          white_time = value;
        } else if (token == "btime") {
          black_time = value;
        } else if (token == "winc") {
          white_inc = value;
        } else if (token == "binc") {
          black_inc = value;
        } else if (token == "movestogo") {
          moves_to_go_val = value;
        } else if (token == "depth") {
          depth_val = value;
        } else if (token == "nodes") {
          nodes_val = value;
        } else if (token == "mate") {
          mate_val = value;
        } else if (token == "movetime") {
          move_time_val = value;
        } else {
          sendWarning("Invalid go command format: " + token);
          return;
        }
      }
    }

    // ponder
    if (ponder_flag) goPonder();

    // time controll
    if (move_time_val)
      goTime(*move_time_val);
    else if (white_time && black_time)
      goTimeControl(white_time.value(), black_time.value(),
                    white_inc.value_or(0), black_inc.value_or(0));
    else if (infinite_flag)
      goInfinite();
    else
      goInfinite();  // default dehaviour

    if (moves_to_go_val) goMovesToGo(*moves_to_go_val);
    if (depth_val) goDepth(*depth_val);
    if (nodes_val) goNodes(*nodes_val);
    if (!move_list.empty()) goMoves(move_list);

    // search mode
    if (perft_flag && perft_depth)
      goPerft(*perft_depth);
    else if (mate_val)
      goMate(*mate_val);
    else
      go();
  }

  void parsePerft(std::istringstream& stream) {
    int depth;
    stream >> depth;
    goPerft(depth);
  }
};
