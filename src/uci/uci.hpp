#include <iostream>
#include <string>
#include <vector>

/*
class UCI : public Parser {
 public:
  UCI() : working(false) {}

  void uci() override {
    id();
    uciOk();
  }

  void stop() override {
    if (working) {
      computer.Abort();
      working = false;
    }
  }

  void quit() override {
    exit(0);
  }

  void isReady() override {
    readyOk();
  }

  void ponderHit() override {
  }

  void uciNewGame() override {
    if (!working) {
      computer.Reset();
    }
  }

  void setOption(const std::string& name, const std::string& value) override {
    computer.SetOption(name, value);
  }

  void position(const std::string& fen, const std::vector<std::string>& moves) override {
    if (!working) {
      Match match;
      if (fen == "startpos") {
        match.LoadFromStartPosition();
      } else {
        match.LoadFromFEN(fen);
      }

      for (const auto& move : moves) {
        match.MakeMove(move);
      }

      computer.SetBoard(match.GetBoard());
    }
  }

  void go(const std::string& parameters) override {
    if (!working) {
      computer.Start(parameters);
      working = true;
    }
  }

  void exec() {
    if (working && computer.IsReady()) {
      bestMove(computer.Get());
      computer.Abort();
      working = false;
    }
    std::cout.flush();
  }

 private:
  Computer computer;
  bool working;

  void id() {
    Writter::idName("MyChessEngine");
    Writter::idAuthor("AuthorName");
  }

  void uciOk() {
    Writter::uciOk();
  }

  void readyOk() {
    Writter::readyOk();
  }

  void bestMove(const std::string& move, const std::string& ponder = "") {
    Writter::bestMove(move, ponder);
  }
};
*/
